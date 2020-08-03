#include "LazyployLauncherClient.h"
#include "LazyployLauncherClientApp.h"

static FString _BuildUploadEndpoint = TEXT("");
static FString _BuildManagerURL = TEXT("http://localhost/");

FLazyployLauncherClient::FLazyployLauncherClient()
	: BuildUploadEndpoint(_BuildUploadEndpoint)
	, BuildManagerURL(_BuildManagerURL)
	, bUpdateBuildStatus(false)
{
}


FLazyployLauncherClient::~FLazyployLauncherClient()
{
}

void FLazyployLauncherClient::RequestCloseWindow(const TSharedRef<SWindow>& Window)
{
}

FString FLazyployLauncherClient::GetEditorBinaryPath() const
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

FString FLazyployLauncherClient::GetEngineBatchFilesPath() const
{
	return FPaths::EngineDir() / TEXT("Build/BatchFiles");
}

FString FLazyployLauncherClient::GetProjectPath() const
{
	return ProjectPath;
}

void FLazyployLauncherClient::SetProjectPath(const FString NewProjectPath)
{
	ProjectPath = NewProjectPath;
}

FString FLazyployLauncherClient::GetProjectDir() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetPath(ProjectPath);
	}
	return FString();
}

FString FLazyployLauncherClient::GetProjectName() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetBaseFilename(ProjectPath);
	}
	return FString();
}

void FLazyployLauncherClient::SetBuildUploadEndpoint(FString NewEndpoint)
{
	_BuildUploadEndpoint = NewEndpoint;
}

void FLazyployLauncherClient::SetBuildManagerURL(FString NewURL)
{
	_BuildManagerURL = NewURL;
}

bool FLazyployLauncherClient::IsProjectSelected() const
{
	return !ProjectPath.IsEmpty();
}

FReply FLazyployLauncherClient::LaunchEditor()
{
	uint32 ProcessID;
	FString Args = GetProjectPath() + TEXT(" -SessionFilter=Lazyploy");
	FPlatformProcess::CreateProc(*GetEditorBinaryPath(), *Args, true, false, false, &ProcessID, 0, nullptr, nullptr);
	return FReply::Handled();
}
