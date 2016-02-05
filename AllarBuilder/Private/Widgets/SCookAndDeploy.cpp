#include "../AllarBuilderClientApp.h"
#include "ISourceCodeAccessModule.h"
#include "SClientLauncher.h"
#include "SCookProgress.h"
#include "SCookAndDeploy.h"
#include "../GenericHttpJsonTask.h"

#define LOCTEXT_NAMESPACE "CookAndDeploy"

void SCookAndDeploy::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
{
	Client = InClient;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.WidthOverride(100)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("CookAndDeploy", "Cook"))
				.TextStyle(InStyle, TEXT("Section.Label"))
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(5.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.HAlign(HAlign_Left)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				[
					SNew(SBox)
					.WidthOverride(650)
					[
						SNew(SGridPanel)
						.FillColumn(0, 0.5f)
						.FillColumn(1, 1)
						.FillColumn(2, 1)
						.FillColumn(3, 1)
						.FillColumn(4, 1)
						// Platforms label
						+ SGridPanel::Slot(0,0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Platforms", "Platforms:"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						// Windows
						+ SGridPanel::Slot(1,0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(WindowsCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("Windows", "Windows"))
							.RightAlignCheckBox(true)
						]
						// WindowsServer
						+ SGridPanel::Slot(2,0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(WindowsServerCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("WindowsServer", "WinServer"))
							.RightAlignCheckBox(true)
						]
						// Linux
						+ SGridPanel::Slot(3,0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(LinuxCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("Linux", "Linux"))
							.RightAlignCheckBox(true)
						]
						// LinuxServer
						+ SGridPanel::Slot(4,0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(LinuxServerCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("LinuxServer", "LinuxServer"))
							.RightAlignCheckBox(true)
						]
						// Row spacer		
						+ SGridPanel::Slot(0, 1)
						[
							SNew(SSpacer)
							.Size(FVector2D(8, 8))
						]
						// Options label			
						+ SGridPanel::Slot(0,2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Options", "Options:"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						// Use Pak files?
						+ SGridPanel::Slot(1,2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(PakCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("UsePakFiles", "Use .pak"))
							.CheckboxState(ECheckBoxState::Checked)
							.RightAlignCheckBox(true)
						]
						// Compress?
						+ SGridPanel::Slot(2,2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(CompressCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("Compress", "Compress"))
							.CheckboxState(ECheckBoxState::Checked)
							.RightAlignCheckBox(true)
						]
						// Iterate?
						+ SGridPanel::Slot(3,2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(IterateCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("Iterate", "Iterate"))
							.RightAlignCheckBox(true)
						]
						// Strip Version?
						+ SGridPanel::Slot(4,2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(StripVersionCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("StripVersion", "StripVersion"))
							.CheckboxState(ECheckBoxState::Checked)
							.RightAlignCheckBox(true)
						]
					]
				]
			]
			// Post Cook Options
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.HAlign(HAlign_Left)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				[
					SNew(SBox)
					.WidthOverride(650)
					[
						SNew(SGridPanel)
						.FillColumn(0, 1)
						.FillColumn(1, 1)
						.FillColumn(2, 1)
						.FillColumn(3, 1)
						.FillColumn(4, 1)
						// Windows Server Steam Fix
						+ SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.ColumnSpan(2)
						[
							SAssignNew(WinServerSteamFixCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("WinServerSteamFix", "Fix for Steam WinServer Binaries"))
							.CheckboxState(ECheckBoxState::Checked)
						]
						// Linux Server Steam Fix
						+ SGridPanel::Slot(2, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(LinuxServerSteamFixCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("LinuxServerSteamFix", "Linux?"))
							.CheckboxState(ECheckBoxState::Checked)
						]
					]
				]
			]
			// Post Stage
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.HAlign(HAlign_Left)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				[
					SNew(SBox)
					.WidthOverride(650)
					[
						SNew(SGridPanel)
						.FillColumn(0, 1)
						.FillColumn(1, 1)
						.FillColumn(2, 1)
						.FillColumn(3, 1)
						.FillColumn(4, 1)
						// Strip Debug Files
						+ SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(StripDebugFilesCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("StripDebugFiles", "Strip Debug"))
							.CheckboxState(ECheckBoxState::Checked)
						]
						// Zip Builds
						+ SGridPanel::Slot(1, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(ZipBuildCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("ZipBuilds", "Zip Builds"))
							.CheckboxState(ECheckBoxState::Checked)
						]
						// Deploy to Build Manager
						+ SGridPanel::Slot(2, 0)
						.ColumnSpan(2)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(DeployToBuildManagerCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("DeployToBuildManager", "Deploy to Build Manager"))
							.CheckboxState(ECheckBoxState::Checked)
						]
					]
				]
			]
			// Start Cook
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.HAlign(HAlign_Left)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				[
					SNew(SBox)
					.WidthOverride(650)
					[
						SNew(SGridPanel)
						.FillColumn(0, 1)
						.FillColumn(1, 1)
						.FillColumn(2, 1)
						.FillColumn(3, 1)
						.FillColumn(4, 1)
						// Begin Cook Button
						+ SGridPanel::Slot(0, 4)
						.ColumnSpan(5)
						[
							SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.IsEnabled(this, &SCookAndDeploy::IsCookingEnabled)
							.OnClicked(this, &SCookAndDeploy::StartCook)
							.Content()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("StartCook", "Start Cook"))
							]
						]
					]
				]
			]
			// Cook Progress
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SAssignNew(CookProgress, SCookProgress, InClient, InStyle)
			]
		]
	];
}

bool SCookAndDeploy::IsCookingEnabled() const
{
	// Ensure at least one platform is selected
	if (!(WindowsCheckboxOption->CheckBox->IsChecked() || WindowsServerCheckboxOption->CheckBox->IsChecked() || LinuxCheckboxOption->CheckBox->IsChecked() || LinuxServerCheckboxOption->CheckBox->IsChecked()))
	{
		return false;
	}

	if (CookProgress.IsValid())
	{
		return CookProgress->IsReadyForNewJob();
	}

	return false;
}

FReply SCookAndDeploy::StartCook()
{
	if (CookProgress.IsValid() && CookProgress->IsReadyForNewJob())
	{
		CookProgress->ClearTasks();

		FString UATPath = FPaths::ConvertRelativePathToFull(Client->GetEngineBatchFilesPath() / TEXT("RunUAT.bat"));
		FPaths::MakePlatformFilename(UATPath);

		FString CookArgs = TEXT("BuildCookRun");
		CookArgs += TEXT(" -project=\"") + Client->GetProjectPath() + TEXT("\"");
		CookArgs += TEXT(" -noP4 -nocompileeditor -utf8output -cook -map= -stage -package -clientconfig=Development -serverconfig=Development");

		// Prevent compiling of UAT if using a Rocket build
		if (FRocketSupport::IsRocket())
		{
			CookArgs += TEXT(" -rocket -nocompile");
		}

		// See if project has code that needs to be built
		FString ProjectPath = FPaths::GetPath(Client->GetProjectPath());
		TArray<FString> OutProjectCodeFilenames;
		IFileManager::Get().FindFilesRecursive(OutProjectCodeFilenames, *(ProjectPath / TEXT("Source")), TEXT("*.h"), true, false, false);
		IFileManager::Get().FindFilesRecursive(OutProjectCodeFilenames, *(ProjectPath / TEXT("Source")), TEXT("*.cpp"), true, false, false);
		ISourceCodeAccessModule& SourceCodeAccessModule = FModuleManager::LoadModuleChecked<ISourceCodeAccessModule>("SourceCodeAccess");
		if (OutProjectCodeFilenames.Num() > 0 && SourceCodeAccessModule.GetAccessor().CanAccessSourceCode())
		{
			CookArgs += TEXT(" -build");
		}	

		// Strip versions from content?
		if (StripVersionCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -unversionedcookedcontent");
		}

		// Use .pak files?
		if (StripVersionCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -pak");
		}

		// Iterative cook?
		if (IterateCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -iterativecooking");
		}

		// Compress build?
		if (CompressCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -compressed");
		}

		bool bWindows = WindowsCheckboxOption->CheckBox->IsChecked();
		bool bWindowsServer = WindowsServerCheckboxOption->CheckBox->IsChecked();
		bool bLinux = LinuxCheckboxOption->CheckBox->IsChecked();
		bool bLinuxServer = LinuxServerCheckboxOption->CheckBox->IsChecked();
		bool bStripDebug = StripDebugFilesCheckboxOption->CheckBox->IsChecked();
		bool bZipBuilds = ZipBuildCheckboxOption->CheckBox->IsChecked();
		bool bDeployToBuildManager = DeployToBuildManagerCheckboxOption->CheckBox->IsChecked();

		FString ProjectEndpoint = FString::Printf(TEXT("http://localhost:3000/api/projects/%s"), *Client->GetProjectName());
		FString ProjectUploadEndpoint = ProjectEndpoint / TEXT("build/upload");

		if (bZipBuilds && bDeployToBuildManager)
		{
			class FGetNewBuildInfoTask : public FGenericHttpJsonTask
			{
			public:
				FGetNewBuildInfoTask(TSharedPtr<FAllarBuilderClient> InClient, const FString& NewBuildEndpoint, FString InProjectUploadEndpoint)
					: FGenericHttpJsonTask(TEXT("GetBuildInfo"), TEXT("Get Build Info from Build Manager"), NewBuildEndpoint)
					, Client(InClient)
					, UploadEndpoint(InProjectUploadEndpoint)
				{
				}
			protected:
				virtual bool PerformTask() override
				{
					if (FGenericHttpJsonTask::PerformTask())
					{	
						const TSharedPtr<FJsonObject>* BuildJson;
						if (JsonObj->TryGetObjectField(TEXT("build"), BuildJson))
						{
							if ((*BuildJson)->TryGetNumberField(TEXT("id"), Client->BuildId))
							{
								Client->SetBuildUploadEndpoint(UploadEndpoint / FString::FromInt(Client->BuildId));
								OnMessageRecieved().Broadcast(FString::Printf(TEXT("Successfully got new build info. Starting build with id: %d"), Client->BuildId));
								return true;
							}
						}
					}

					OnMessageRecieved().Broadcast(TEXT("Failed to get build info from Build Manager"));
					return false;
				}
			private:
				TSharedPtr<FAllarBuilderClient> Client;
				FString UploadEndpoint;
			};

			FString BuildDesc = TEXT("Automated build test.");
			CookProgress->AddTask(MakeShareable(new FGetNewBuildInfoTask(Client, ProjectEndpoint / TEXT("build/new") / BuildDesc, ProjectUploadEndpoint)));
		}

		// Build Windows Tasks
		if ( bWindows || bWindowsServer )
		{
			// Set up Cook task
			FString PlatformString;
			FString TaskDesc;
			if (bWindows)
			{
				PlatformString += TEXT(" -platform=Win64");
				TaskDesc += TEXT("Win64");
			}

			if (bWindowsServer)
			{
				PlatformString += TEXT(" -server -serverplatform=Win64");
				TaskDesc += TaskDesc.Len() > 0 ? TEXT(" and ") : TEXT("");
				TaskDesc += ("Win64 Server");
			}

			CookProgress->NewTask(TEXT("WindowsCook"), TEXT("Cooking ") + TaskDesc, UATPath, CookArgs + PlatformString, Client->GetEngineBatchFilesPath());

			// Set up Post Cook Tasks
			FString StagedBuildDir = Client->GetProjectDir() / TEXT("Saved/StagedBuilds");
			
			if (bWindows)
			{
				const FString PlatformDir = TEXT("WindowsNoEditor");
				const FString BuildDir = StagedBuildDir / PlatformDir;
				
				if (bStripDebug)
				{
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { get-childitem \"%s\" -include *.pdb -recurse | foreach ($_) {remove-item $_.fullname} }\""), *BuildDir);
					CookProgress->NewTask(TEXT("StripDebugWindows"), TEXT("Strip Debug Files for Windows"), TEXT("powershell.exe"), CommandArgs, TEXT(""));
				}
				
				if (bZipBuilds)
				{
					static FString BuildArchivePath = FPaths::Combine(*StagedBuildDir, *(PlatformDir + TEXT(".zip")));
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BuildArchivePath);
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; Try { [IO.Compression.ZipFile]::CreateFromDirectory('%s', '%s.zip'); } Catch { echo $_.Exception|format-list -force; exit 1; } }\""), *PlatformDir, *PlatformDir);
					CookProgress->NewTask(TEXT("ZipWindows"), TEXT("Zip Windows Build"), TEXT("powershell.exe"), CommandArgs, StagedBuildDir);

					if (DeployToBuildManagerCheckboxOption->CheckBox->IsChecked())
					{
						CookProgress->NewTask(TEXT("UploadWindows"), TEXT("Upload Windows Build"), Client->BuildUploadEndpoint, BuildArchivePath);
					}
				}		
			}
			if (bWindowsServer)
			{
				const FString PlatformDir = TEXT("WindowsServer");
				const FString BuildDir = StagedBuildDir / PlatformDir;

				if (bStripDebug)
				{
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { get-childitem \"%s\" -include *.pdb -recurse | foreach ($_) {remove-item $_.fullname} }\""), *BuildDir);
					CookProgress->NewTask(TEXT("StripDebugWindowsServer"), TEXT("Strip Debug Files for Windows Server"), TEXT("powershell.exe"), CommandArgs, TEXT(""));
				}
				
				if (bZipBuilds)
				{
					static FString BuildArchivePath = FPaths::Combine(*StagedBuildDir, *(PlatformDir + TEXT(".zip")));
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BuildArchivePath);
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; Try { [IO.Compression.ZipFile]::CreateFromDirectory('%s', '%s.zip'); } Catch { echo $_.Exception|format-list -force; exit 1; } }\""), *PlatformDir, *PlatformDir);
					CookProgress->NewTask(TEXT("ZipWindowsServer"), TEXT("Zip Windows Server Build"), TEXT("powershell.exe"), CommandArgs, StagedBuildDir);

					if (DeployToBuildManagerCheckboxOption->CheckBox->IsChecked())
					{
						CookProgress->NewTask(TEXT("UploadWindowsServer"), TEXT("Upload Windows Server Build"), Client->BuildUploadEndpoint, BuildArchivePath);
					}
				}
			}
		}

		// Build Linux Task
		if (LinuxCheckboxOption->CheckBox->IsChecked() || LinuxServerCheckboxOption->CheckBox->IsChecked())
		{
			FString PlatformString;
			FString TaskDesc;
			if (LinuxCheckboxOption->CheckBox->IsChecked())
			{
				PlatformString += TEXT(" -platform=Linux");
				TaskDesc += TEXT("Linux");
			}

			if (LinuxServerCheckboxOption->CheckBox->IsChecked())
			{
				PlatformString += TEXT(" -server -serverplatform=Linux");
				TaskDesc += TaskDesc.Len() > 0 ? TEXT(" and ") : TEXT("");
				TaskDesc += ("Linux Server");
			}

			CookProgress->NewTask(TEXT("LinuxCook"), TEXT("Cooking ") + TaskDesc, UATPath, CookArgs + PlatformString, Client->GetEngineBatchFilesPath());
		
			// Set up Post Cook Tasks
			FString StagedBuildDir = Client->GetProjectDir() / TEXT("Saved/StagedBuilds");

			if (bLinux)
			{
				const FString PlatformDir = TEXT("LinuxNoEditor");
				const FString BuildDir = StagedBuildDir / PlatformDir;

				if (bStripDebug)
				{
					// @TODO: Strip Linux Debug Files
				}

				if (bZipBuilds)
				{
					static FString BuildArchivePath = FPaths::Combine(*StagedBuildDir, *(PlatformDir + TEXT(".zip")));
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BuildArchivePath);
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; Try { [IO.Compression.ZipFile]::CreateFromDirectory('%s', '%s.zip'); } Catch { echo $_.Exception|format-list -force; exit 1; } }\""), *PlatformDir, *PlatformDir);
					CookProgress->NewTask(TEXT("ZipLinux"), TEXT("Zip Linux Build"), TEXT("powershell.exe"), CommandArgs, StagedBuildDir);

					if (DeployToBuildManagerCheckboxOption->CheckBox->IsChecked())
					{
						CookProgress->NewTask(TEXT("UploadLinux"), TEXT("Upload Linux Build"), Client->BuildUploadEndpoint, BuildArchivePath);
					}
				}
			}
			if (bLinuxServer)
			{
				const FString PlatformDir = TEXT("LinuxServer");
				const FString BuildDir = StagedBuildDir / PlatformDir;

				if (bStripDebug)
				{
					// @TODO: Strip Linux Debug Files
				}

				if (bZipBuilds)
				{
					static FString BuildArchivePath = FPaths::Combine(*StagedBuildDir, *(PlatformDir + TEXT(".zip")));
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BuildArchivePath);
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; Try { [IO.Compression.ZipFile]::CreateFromDirectory('%s', '%s.zip'); } Catch { echo $_.Exception|format-list -force; exit 1; } }\""), *PlatformDir, *PlatformDir);
					CookProgress->NewTask(TEXT("ZipLinuxServer"), TEXT("Zip Linux Server Build"), TEXT("powershell.exe"), CommandArgs, StagedBuildDir);

					if (DeployToBuildManagerCheckboxOption->CheckBox->IsChecked())
					{
						CookProgress->NewTask(TEXT("UploadLinux"), TEXT("Upload Linux Server Build"), Client->BuildUploadEndpoint, BuildArchivePath);
					}
				}
			}
		}

		CookProgress->ExecuteTasks();
	}
	
	return FReply::Handled();
}
