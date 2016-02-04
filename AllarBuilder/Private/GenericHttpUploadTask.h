#pragma once

#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "HttpModule.h"

class FGenericHttpUploadTask
	: public FGenericTask
{
public:
	FGenericHttpUploadTask(const FString& InName, const FString& InDesc, const FString& InURL, const FString& InFilePath)
		: FGenericTask(TEXT(""), TEXT(""), TEXT(""), InName, InDesc)
		, TaskFilePath(InFilePath)
		, TaskURL(InURL)
		, PayloadSize(0)
		, bHttpRequestFinished(false)
		, bUploadSucceeded(false)
	{
		// Make sure HttpModule is loaded before we start the threaded process
		FHttpModule::Get().SetHttpTimeout(60.0f);
	}

	void UploadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved)
	{
		OnMessageRecieved().Broadcast(FString::Printf(TEXT("Uploaded %d / %d (%d%%) bytes."), BytesSent, PayloadSize, (int32)(((float)BytesSent/(float)PayloadSize)*100.0f)));
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

		if (HttpResponse->GetResponseCode() == 200)
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
		
		Reader->Serialize(PayloadBytes + HeaderSize, FileSize);
		Reader->Close();
		delete Reader;

		// Ghetto UTF8 to ANSI copy (@TODO: Make better)
		for (int32 i = 0; i < FooterSize * 2; i += 2)
		{
			PayloadBytes[FileSize + HeaderSize + (i/2)] = PayloadFooterBytes[i];
		}
		
		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + DataBoundary);
		//HttpRequest->SetHeader(TEXT("Content-Transfer-Encoding"), TEXT("binary"));
		HttpRequest->SetURL(TaskURL);
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContent(Payload);
		HttpRequest->OnRequestProgress().BindRaw(this, &FGenericHttpUploadTask::UploadRequestProgress);
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FGenericHttpUploadTask::UploadRequestComplete);
		HttpRequest->ProcessRequest();

		Payload.Reset();

		while (!bHttpRequestFinished)
		{
			FPlatformProcess::Sleep(0.25);
		}

		return bUploadSucceeded;
	}

protected:

	FString TaskURL;
	FString TaskFilePath;

	int32 PayloadSize; 

	bool bHttpRequestFinished;
	bool bUploadSucceeded;

};