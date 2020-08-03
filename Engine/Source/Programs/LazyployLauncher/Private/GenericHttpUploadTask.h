#pragma once

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "LazyployLauncherClient.h"

class FGenericHttpUploadTask
	: public FGenericTask
{
public:
	FGenericHttpUploadTask(TWeakPtr<FLazyployLauncherClient> InLazyployClient, const FString& InName, const FString& InDesc, const FString& InPlatform, const FString& InFilePath)
		: FGenericTask(InName, InDesc)
		, LazyployClient(InLazyployClient)
		, Platform(InPlatform)
		, TaskFilePath(InFilePath)
		, PayloadSize(0)
		, bHttpRequestFinished(false)
		, bUploadSucceeded(false)
	{
		// Make sure HttpModule is loaded before we start the threaded process
		FHttpModule::Get().SetHttpTimeout(60.0f);
	}

	void UploadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved)
	{
		//OnMessageRecieved().Broadcast(FString::Printf(TEXT("Uploaded %d / %d (%d%%) bytes."), BytesSent, PayloadSize, (int32)(((float)BytesSent/(float)PayloadSize)*100.0f)));
	}
	
	void UploadRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		if (!HttpResponse.IsValid())
		{
			OnMessageRecieved().Broadcast(FString::Printf(TEXT("Upload failed, request timed out or no response from server.")));
			bHttpRequestFinished = true;
			return;
		}

		OnMessageRecieved().Broadcast(HttpResponse->GetContentAsString());

		const int32 ResponseCode = HttpResponse->GetResponseCode();
		if (ResponseCode >= 200 && ResponseCode <= 299)
		{
			// If server response doesn't include error, mark as upload succeeded
			if (!HttpResponse.Get()->GetContentAsString().Contains(TEXT("Error")))
			{
				bUploadSucceeded = true;
				bHttpRequestFinished = true;
				return;
			}
		}

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("Error: Upload failed, HTTP Error Code %d, response from server: %s"), HttpResponse->GetResponseCode(), *HttpResponse->GetContentAsString()));
		bHttpRequestFinished = true;
	}

	virtual bool PerformTask() override
	{
		check(LazyployClient.IsValid());

		//curl -F "file=@D:/depot/jam/Saved/StagedBuilds/WindowsNoEditor.zip" "http://localhost:3030/buildUploads?buildid=2&platform=Linux"

		FString FullCommandLine = FString::Printf(TEXT("/c curl -F \"file=@%s\" \"%s&platform=%s\""), *TaskFilePath, *LazyployClient.Pin()->BuildUploadEndpoint, *Platform);
		TSharedPtr<FMonitoredProcess> UploadProcess = MakeShareable(new FMonitoredProcess(TEXT("cmd.exe"), FullCommandLine, true));

		FMonitoredProcess Process(TEXT("cmd.exe"), FullCommandLine, true);
        Process.OnOutput().BindLambda([this](const FString& Text){ OnMessageRecieved().Broadcast(Text); });
        Process.Launch();
        while(Process.Update())
        {
        	OnMessageRecieved().Broadcast(TEXT("working..."));
            FPlatformProcess::Sleep(0.1f);
        }

        int32 ReturnCode = Process.GetReturnCode();
        if (ReturnCode != 0)
        {
        	OnMessageRecieved().Broadcast(TEXT("Upload failed."));
            return false;
        }
		
		return true;
	}

	virtual bool PerformOldTask()
	{
		OnMessageRecieved().Broadcast(FString::Printf(TEXT("START TASK")));
		check(LazyployClient.IsValid());

		// Open file for reading
		FArchive* Reader = IFileManager::Get().CreateFileReader(*TaskFilePath, 0);
		if (!Reader)
		{
			OnMessageRecieved().Broadcast(FString::Printf(TEXT("Error: Upload failed. Could not open file for reading: %s"), *TaskFilePath));
			return false;
		}

		FString DataBoundary = TEXT("----867-5309--I-Got-Your-Number");
		FString CRLF = TEXT("\r\n");

		// Build payload header
		FString PayloadHeader = TEXT("--") + DataBoundary + CRLF + TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"") + FPaths::GetCleanFilename(TaskFilePath) + TEXT("\"");
		PayloadHeader += CRLF + TEXT("Content-Type: application/octet-stream") + CRLF /* + TEXT("Content-Transfer-Encoding: binary") + CRLF */ + CRLF;
		uint8* PayloadHeaderBytes = (uint8*)(PayloadHeader.GetCharArray().GetData());
		int32 HeaderSize = PayloadHeader.Len();

		// Build payload footer
		FString PayloadFooter = CRLF + TEXT("--") + DataBoundary + TEXT("--");
		uint8* PayloadFooterBytes = (uint8*)PayloadFooter.GetCharArray().GetData();
		int32 FooterSize = PayloadFooter.Len();

		// Build payload
		TArray<uint8> Payload;
		int64 FileSize = Reader->TotalSize();
		PayloadSize = FileSize + HeaderSize + FooterSize;
		Payload.AddUninitialized(PayloadSize);
		uint8* PayloadBytes = Payload.GetData();

		// Ghetto UTF8 to ANSI copy (@TODO: Make better)
		for (int32 i = 0; i < HeaderSize * 2; i += 2)
		{
			PayloadBytes[i/2] = PayloadHeaderBytes[i];
		}

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("PRE-SERIALIZED")));
		
		Reader->Serialize(PayloadBytes + HeaderSize, FileSize);
		Reader->Close();
		delete Reader;

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("POST-SERIALIZED")));

		// Ghetto UTF8 to ANSI copy (@TODO: Make better)
		for (int32 i = 0; i < FooterSize * 2; i += 2)
		{
			PayloadBytes[FileSize + HeaderSize + (i/2)] = PayloadFooterBytes[i];
		}
		
		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + DataBoundary);
		HttpRequest->SetURL(LazyployClient.Pin()->BuildUploadEndpoint + TEXT("&platform=") + Platform);
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContent(Payload);
		HttpRequest->OnRequestProgress().BindRaw(this, &FGenericHttpUploadTask::UploadRequestProgress);
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FGenericHttpUploadTask::UploadRequestComplete);
		HttpRequest->ProcessRequest();

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("Pre-Sleep")));

		Payload.Reset();

		while (!bHttpRequestFinished)
		{
			FPlatformProcess::Sleep(0.25);
		}

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("DONE")));

		return bUploadSucceeded;
	}

protected:

	TWeakPtr<FLazyployLauncherClient> LazyployClient;
	
	const FString Platform;
	const FString& TaskFilePath;

	int32 PayloadSize; 

	bool bHttpRequestFinished;
	bool bUploadSucceeded;
};
