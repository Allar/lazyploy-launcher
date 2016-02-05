#pragma once

#include "IHttpRequest.h"
#include "IHttpResponse.h"
#include "HttpModule.h"

class FGenericHttpJsonTask
	: public FGenericTask
{
public:
	FGenericHttpJsonTask(const FString& InName, const FString& InDesc, const FString& InURL, const FString& InVerb = TEXT("GET"), const FString& InContent = TEXT(""))
		: FGenericTask(InName, InDesc)
		, URL(InURL)
		, Verb(InVerb)
		, Content(InContent)
		, bHttpRequestFinished(false)
		, JsonObj(nullptr)
	{
		// Make sure HttpModule is loaded before we start the threaded process
		FHttpModule::Get().SetHttpTimeout(60.0f);
	}

	void HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		if (!HttpResponse.IsValid())
		{
			OnMessageRecieved().Broadcast(FString::Printf(TEXT("Error: Http Json request timed out or no response from server.")));
			bHttpRequestFinished = true;
			return;
		}

		if (HttpResponse->GetResponseCode() == 200)
		{
			FString ResponseContent = HttpResponse->GetContentAsString();

			// Try to deserialize data to JSON
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
			FJsonSerializer::Deserialize(JsonReader, JsonObj);

			// Zero is success
			Result = !(JsonObj.IsValid());
			bHttpRequestFinished = true;
			return;
		}

		OnMessageRecieved().Broadcast(FString::Printf(TEXT("Error: Http Json request failed with status %d, response from server: %s"), HttpResponse->GetResponseCode(), *HttpResponse->GetContentAsString()));
		bHttpRequestFinished = true;
	}

	virtual bool PerformTask() override
	{
		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		HttpRequest->SetURL(URL);
		HttpRequest->SetVerb(Verb);
		if (Content.Len() > 0)
		{
			HttpRequest->SetContentAsString(Content);
		}
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FGenericHttpJsonTask::HttpRequestComplete);
		HttpRequest->ProcessRequest();

		while (!bHttpRequestFinished)
		{
			FPlatformProcess::Sleep(0.25);
		}

		return Result == 0;
	}

protected:

	FString URL;
	FString Content;
	FString Verb;

	bool bHttpRequestFinished;

	/** JSON data */
	TSharedPtr<FJsonObject> JsonObj;
};