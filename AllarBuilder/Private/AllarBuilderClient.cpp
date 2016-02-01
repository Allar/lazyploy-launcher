#include "AllarBuilderClientApp.h"
#include "AllarBuilderClient.h"
#include "UniquePtr.h"

#include "TaskGraphInterfaces.h"

FAllarBuilderClient::FAllarBuilderClient()
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

FString FAllarBuilderClient::GetProjectPath() const
{
	return ProjectPath;
}

void FAllarBuilderClient::SetProjectPath(const FString NewProjectPath)
{
	ProjectPath = NewProjectPath;
}

FString FAllarBuilderClient::GetProjectName() const
{
	if (!ProjectPath.IsEmpty())
	{
		return FPaths::GetBaseFilename(ProjectPath);
	}
	return FString();
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
