#include "AllarBuilderClientApp.h"
#include "AllarBuilderClient.h"
#include "UniquePtr.h"

#include "TaskGraphInterfaces.h"

FAllarBuilderClient::FAllarBuilderClient()
	: bLaunchListenServer(false), bAutoConnect(false)
	, NumberOfClients(1), ResX(1280), ResY(720), PosX(0), PosY(0)
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

void FAllarBuilderClient::OnNumberOfClientsChanged(uint32 Value, ETextCommit::Type CommitType)
{
	NumberOfClients = Value;
}

void FAllarBuilderClient::OnListenServerChanged(const ECheckBoxState NewCheckedState)
{
	bLaunchListenServer = (NewCheckedState == ECheckBoxState::Checked);
}

void FAllarBuilderClient::OnAutoConnectChanged(const ECheckBoxState NewCheckedState)
{
	bAutoConnect = (NewCheckedState == ECheckBoxState::Checked);
}

void FAllarBuilderClient::OnClientArgumentsChanged(const FText& InText, ETextCommit::Type InCommitType)
{
	ClientArguments = InText.ToString();
}

void FAllarBuilderClient::OnResXChanged(uint32 Value, ETextCommit::Type CommitType)
{
	ResX = Value;
}

void FAllarBuilderClient::OnResYChanged(uint32 Value, ETextCommit::Type CommitType)
{
	ResY = Value;
}

void FAllarBuilderClient::OnPosXChanged(uint32 Value, ETextCommit::Type CommitType)
{
	PosX = Value;
}

void FAllarBuilderClient::OnPosYChanged(uint32 Value, ETextCommit::Type CommitType)
{
	PosY = Value;
}

bool FAllarBuilderClient::IsAutoConnectEnabled() const
{
	return bLaunchListenServer;
}

FReply FAllarBuilderClient::LaunchClients()
{
	FString EditorBinaryPath = GetEditorBinaryPath();
	uint32 ProcessID;

	FString NewClientArgs;
	for (uint32 i = 0; i < NumberOfClients; ++i)
	{
		// First (host?) client
		if (i == 0)
		{
			NewClientArgs = ClientArguments;
			if (bLaunchListenServer)
			{
				NewClientArgs = (NewClientArgs.Len() == 0) ? TEXT("-listen") : NewClientArgs + TEXT("?Listen");
			}
		}
		// Secondary (client?) clients
		else
		{
			// Ignore provided arguments if launching listen server
			if (bLaunchListenServer)
			{
				// AutoConnect will connect to local listen server
				if (bAutoConnect)
				{
					NewClientArgs = TEXT("127.0.0.1");
				}
				else
				{
					NewClientArgs.Empty();
				}
			}
			else // Use supplied arguments if no server is being created
			{
				NewClientArgs = ClientArguments;
			}
		}

		FString ResAndPosArgs = FString::Printf(TEXT(" -resx=%d -resy=%d -winx=%d -winy=%d"), ResX, ResY, PosX + ((i % 2) * ResX), PosY + ((i / 2) * ResY));

		NewClientArgs = FString::Printf(TEXT("%s %s -game %s"), *GetProjectPath(), *NewClientArgs, *ResAndPosArgs);

		UE_LOG(LogAllarBuilder, Log, TEXT("Launching client with arguments: %s"), *NewClientArgs);

		FPlatformProcess::CreateProc(*EditorBinaryPath, *NewClientArgs, true, false, false, &ProcessID, 0, nullptr, nullptr);
	}
	return FReply::Handled();
}
