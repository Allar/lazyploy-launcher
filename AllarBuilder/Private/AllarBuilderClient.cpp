#include "AllarBuilderClientApp.h"
#include "AllarBuilderClient.h"
#include "UniquePtr.h"

#include "TaskGraphInterfaces.h"

static FString _BuildUploadEndpoint = TEXT("");
static FString _BuildManagerURL = TEXT("http://localhost/");

FAllarBuilderClient::FAllarBuilderClient()
	: BuildUploadEndpoint(_BuildUploadEndpoint)
	, BuildManagerURL(_BuildManagerURL)
	, bUpdateBuildStatus(false)
{
}


FAllarBuilderClient::~FAllarBuilderClient()
{
}

void FAllarBuilderClient::RequestCloseWindow(const TSharedRef<SWindow>& Window)
{
}

FString FAllarBuilderClient::GetEditorBinaryPath() const
{
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	return FPaths::EngineDir() / TEXT("Binaries/Win64/UE4Editor.exe");
#else
	return = FPaths::EngineDir() / TEXT("Binaries/Win32/UE4Editor.exe");
#endif
#endif
	return FString("Not implemented for this platform.");
}

FString FAllarBuilderClient::GetEngineBatchFilesPath() const
{
	return FPaths::EngineDir() / TEXT("Build/BatchFiles");
}

FString FAllarBuilderClient::GetProjectPath() const
{
	return ProjectPath;
}

void FAllarBuilderClient::SetProjectPath(const FString NewProjectPath)
{
	ProjectPath = NewProjectPath;
}

FString FAllarBuilderClient::GetProjectDir() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetPath(ProjectPath);
	}
	return FString();
}

FString FAllarBuilderClient::GetProjectName() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetBaseFilename(ProjectPath);
	}
	return FString();
}

void FAllarBuilderClient::SetBuildUploadEndpoint(FString NewEndpoint)
{
	_BuildUploadEndpoint = NewEndpoint;
}

void FAllarBuilderClient::SetBuildManagerURL(FString NewURL)
{
	_BuildManagerURL = NewURL;
}

bool FAllarBuilderClient::IsProjectSelected() const
{
	return !ProjectPath.IsEmpty();
}

FReply FAllarBuilderClient::LaunchEditor()
{
	uint32 ProcessID;
	FPlatformProcess::CreateProc(*GetEditorBinaryPath(), *GetProjectPath(), true, false, false, &ProcessID, 0, nullptr, nullptr);
	return FReply::Handled();
}
