#include "../LazyployLauncherClientApp.h"
#include "ISourceCodeAccessModule.h"
#include "SClientLauncher.h"
#include "SCookProgress.h"
#include "SCookAndDeploy.h"
#include "../GenericHttpJsonTask.h"
#include "../GenericTaskCollection.h"
#include "Misc/EngineBuildSettings.h"

#define LOCTEXT_NAMESPACE "CookAndDeploy"

void SCookAndDeploy::Construct(const FArguments& InArgs, TSharedRef<FLazyployLauncherClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
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
							.CheckboxState(ECheckBoxState::Checked)
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
			// More cook options
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
						// Refresh Binaries Only
						+ SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.ColumnSpan(2)
						[
							SAssignNew(RefreshBinariesOnlyCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("RefreshBinariesOnly", "Refresh Binaries Only"))
							.CheckboxState(ECheckBoxState::Unchecked)
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
							.IsEnabled(this, &SCookAndDeploy::IsWinServerSteamFixEnabled)
							.LabelText(LOCTEXT("WinServerSteamFix", "Fix Steam WinServer Binaries"))
							.CheckboxState(ECheckBoxState::Checked)
						]
						// Linux Server Steam Fix
						+ SGridPanel::Slot(2, 0)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.ColumnSpan(2)
						[
							SAssignNew(LinuxServerSteamFixCheckboxOption, SCheckboxOption, InStyle)
							.IsEnabled(this, &SCookAndDeploy::IsLinuxServerSteamFixEnabled)
							.LabelText(LOCTEXT("LinuxServerSteamFix", "Fix Steam LinuxServer Binaries"))
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
							.CheckboxState(ECheckBoxState::Unchecked)
							.IsEnabled(this, &SCookAndDeploy::IsBuildManagerEnabled)
						]
					]
				]
			]
			// Build Manager Settings
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.HAlign(HAlign_Left)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				.IsEnabled(this, &SCookAndDeploy::IsBuildManagerSettingsEnabled)
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
						// Build Manager Url
						+ SGridPanel::Slot(0, 0)
						.ColumnSpan(1)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BuildManagerUrl", "Build Manager Url:"))
				.			TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SGridPanel::Slot(1, 0)
						.ColumnSpan(4)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(BuildManagerUrlTextBox, SEditableTextBox)
							.Text(FText::FromString("http://localhost/"))
							.MinDesiredWidth(600)
						]
						// Row spacing
						+ SGridPanel::Slot(0, 1)
						.ColumnSpan(5)
						[
							SNew(SSpacer)
							.Size(FVector2D(8.0f, 8.0f))
						]
						// Build Description
						+ SGridPanel::Slot(0, 2)
						.ColumnSpan(1)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BuildDescription", "Build Description:"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SGridPanel::Slot(1, 2)
						.ColumnSpan(4)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SAssignNew(BuildDescriptionTextBox, SEditableTextBox)
							.Text(FText::FromString(""))
							.MinDesiredWidth(600)
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

	LoadOptionsFromConfig();
}

// Probably the worst way to do this.
// @TODO: Make a data storage class
void SCookAndDeploy::LoadOptionsFromConfig()
{
	// @HACK: SetIsChecked would not take a bool. Is there a better way?
#define BOOL_TO_CHECKED(inBool) inBool ? ECheckBoxState::Checked : ECheckBoxState::Unchecked

	// Default Cook Options
	bool bWindowsEnabled = false;
	bool bWindowsServerEnabled = false;
	bool bLinuxEnabled = false;
	bool bLinuxServerEnabled = false;
	bool bPakEnabled = true;
	bool bCompressEnabled = true;
	bool bIterateEnabled = true;
	bool bStripVersionEnabled = true;

	// Load Default Cook Options
	GConfig->GetBool(TEXT("Lazyploy.Platforms"), TEXT("bWindows"), bWindowsEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Platforms"), TEXT("bWindowsServer"), bWindowsServerEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Platforms"), TEXT("bLinux"), bLinuxEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Platforms"), TEXT("bLinuxServer"), bLinuxServerEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Options"), TEXT("bPak"), bPakEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Options"), TEXT("bCompress"), bCompressEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Options"), TEXT("bIterate"), bIterateEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Options"), TEXT("bStripVersion"), bStripVersionEnabled, GEngineIni);

	WindowsCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bWindowsEnabled));
	WindowsServerCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bWindowsServerEnabled));
	LinuxCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bLinuxEnabled));
	LinuxServerCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bLinuxServerEnabled));
	PakCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bPakEnabled));
	CompressCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bCompressEnabled));
	IterateCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bIterateEnabled));
	StripVersionCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bStripVersionEnabled));

	// Default More Cook Options
	bool bRefreshBinariesOnly = false;

	// Load More Cook Options
	GConfig->GetBool(TEXT("Lazyploy.Options"), TEXT("bRefreshBinariesOnly"), bRefreshBinariesOnly, GEngineIni);

	RefreshBinariesOnlyCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bRefreshBinariesOnly));

	// Default Steam Fix Options
	bool bSteamFixWinServerEnabled = false;
	bool bSteamFixLinuxServerEnabled = false;

	GConfig->GetBool(TEXT("Lazyploy.SteamFix"), TEXT("bWindows"), bSteamFixWinServerEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.SteamFix"), TEXT("bLinux"), bSteamFixLinuxServerEnabled, GEngineIni);

	WinServerSteamFixCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bSteamFixWinServerEnabled));
	LinuxServerSteamFixCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bSteamFixLinuxServerEnabled));

	// Default Post Stage Options
	bool bStripDebugFilesEnabled = true;
	bool bZipBuildsEnabled = true;
	bool bDeployToLazyployEnabled = false;

	GConfig->GetBool(TEXT("Lazyploy.PostStage"), TEXT("bStripDebug"), bStripDebugFilesEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.PostStage"), TEXT("bZip"), bZipBuildsEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.PostStage"), TEXT("bDeployLazyploy"), bDeployToLazyployEnabled, GEngineIni);

	StripDebugFilesCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bStripDebugFilesEnabled));
	ZipBuildCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bZipBuildsEnabled));
	DeployToBuildManagerCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bDeployToLazyployEnabled));

	// Default Lazyploy options
	FString LazyployUrl = TEXT("http://localhost/");
	FString BuildDescription = TEXT("");

	GConfig->GetString(TEXT("Lazyploy.PostStage"), TEXT("LazyployUrl"), LazyployUrl, GEngineIni);
	GConfig->GetString(TEXT("Lazyploy.PostStage"), TEXT("BuildDescription"), BuildDescription, GEngineIni);

	BuildManagerUrlTextBox->SetText(FText::FromString(LazyployUrl));
	BuildDescriptionTextBox->SetText(FText::FromString(BuildDescription));

#undef BOOL_TO_CHECKED
}

void SCookAndDeploy::SaveOptionsToConfig()
{
	// Save Default Cook Options
	GConfig->SetBool(TEXT("Lazyploy.Platforms"), TEXT("bWindows"), WindowsCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Platforms"), TEXT("bWindowsServer"), WindowsServerCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Platforms"), TEXT("bLinux"), LinuxCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Platforms"), TEXT("bLinuxServer"), LinuxServerCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Options"), TEXT("bPak"), PakCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Options"), TEXT("bCompress"), CompressCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Options"), TEXT("bIterate"), IterateCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Options"), TEXT("bStripVersion"), StripVersionCheckboxOption->CheckBox->IsChecked(), GEngineIni);

	// Save More Cook Options
	GConfig->SetBool(TEXT("Lazyploy.Options"), TEXT("bRefreshBinariesOnly"), RefreshBinariesOnlyCheckboxOption->CheckBox->IsChecked(), GEngineIni);

	// Default Steam Fix Options
	GConfig->SetBool(TEXT("Lazyploy.SteamFix"), TEXT("bWindows"), WinServerSteamFixCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.SteamFix"), TEXT("bLinux"), LinuxServerSteamFixCheckboxOption->CheckBox->IsChecked(), GEngineIni);

	// Default Post Stage Options
	GConfig->SetBool(TEXT("Lazyploy.PostStage"), TEXT("bStripDebug"), StripDebugFilesCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.PostStage"), TEXT("bZip"), ZipBuildCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.PostStage"), TEXT("bDeployLazyploy"), DeployToBuildManagerCheckboxOption->CheckBox->IsChecked(), GEngineIni);

	// Default Lazyploy options
	GConfig->SetString(TEXT("Lazyploy.PostStage"), TEXT("LazyployUrl"), *BuildManagerUrlTextBox->GetText().ToString(), GEngineIni);
	GConfig->SetString(TEXT("Lazyploy.PostStage"), TEXT("BuildDescription"), *BuildDescriptionTextBox->GetText().ToString(), GEngineIni);

	GConfig->Flush(false, GEngineIni);
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

bool SCookAndDeploy::IsWinServerSteamFixEnabled() const
{
	return WindowsServerCheckboxOption->CheckBox->IsChecked();
}

bool SCookAndDeploy::IsLinuxServerSteamFixEnabled() const
{
	return LinuxServerCheckboxOption->CheckBox->IsChecked();
}

bool SCookAndDeploy::IsBuildManagerEnabled() const
{
	return ZipBuildCheckboxOption->CheckBox->IsChecked();
}

bool SCookAndDeploy::IsBuildManagerSettingsEnabled() const
{
	return ZipBuildCheckboxOption->CheckBox->IsChecked() && DeployToBuildManagerCheckboxOption->CheckBox->IsChecked();
}

FReply SCookAndDeploy::StartCook()
{
	if (CookProgress.IsValid() && CookProgress->IsReadyForNewJob())
	{
		SaveOptionsToConfig();

		CookProgress->ClearTasks();

		// Determine Steam SDK if possible
		FString SteamSDK = TEXT("");
		{
			FString SteamworksBuildFileContents;
			FFileHelper::LoadFileToString(SteamworksBuildFileContents, *(FPaths::EngineSourceDir() / TEXT("ThirdParty/Steamworks/Steamworks.build.cs")));

			FString SearchString = TEXT("string SteamVersion = \"");

			int32 SteamVersionIdx = SteamworksBuildFileContents.Find(SearchString, ESearchCase::CaseSensitive);
			if (SteamVersionIdx != INDEX_NONE)
			{
				int32 SteamVersionEndIdx = SteamworksBuildFileContents.Find("\"", ESearchCase::CaseSensitive, ESearchDir::FromStart, SteamVersionIdx + SearchString.Len());
				SteamSDK = TEXT("Steam") + SteamworksBuildFileContents.Mid(SteamVersionIdx + SearchString.Len(), SteamVersionEndIdx - (SteamVersionIdx + SearchString.Len()));
			}
		}
		if (SteamSDK.Len() > 0)
		{
			CookProgress->HandleTaskMessageReceived(FString::Printf(TEXT("Detected Steam SDK as %s"), *SteamSDK));
		}
		else
		{
			CookProgress->HandleTaskMessageReceived(FString::Printf(TEXT("Failed to detect Steam SDK")));
		}
		

		FString UATPath = FPaths::ConvertRelativePathToFull(Client->GetEngineBatchFilesPath() / TEXT("RunUAT.bat"));
		FPaths::MakePlatformFilename(UATPath);

		FString CookArgs = TEXT("BuildCookRun");
		CookArgs += TEXT(" -project=\"") + Client->GetProjectPath() + TEXT("\"");
		CookArgs += TEXT(" -noP4 -nocompileeditor -utf8output -cook -map= -stage -package -clientconfig=Development -serverconfig=Development");

		// Prevent compiling of UAT if using a Rocket build
		if (!FEngineBuildSettings::IsSourceDistribution())
		{
			CookArgs += TEXT(" -rocket -nocompile");
		}

		bool bCodeProject = false;

		// See if project has code that needs to be built
		FString ProjectPath = FPaths::GetPath(Client->GetProjectPath());
		TArray<FString> OutProjectCodeFilenames;
		IFileManager::Get().FindFilesRecursive(OutProjectCodeFilenames, *(ProjectPath / TEXT("Source")), TEXT("*.h"), true, false, false);
		IFileManager::Get().FindFilesRecursive(OutProjectCodeFilenames, *(ProjectPath / TEXT("Source")), TEXT("*.cpp"), true, false, false);
		if (OutProjectCodeFilenames.Num() > 0)
		{
			bCodeProject = true;
			//CookArgs += TEXT(" -build");
		}	

		// Strip versions from content?
		if (StripVersionCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -unversionedcookedcontent");
		}

		// Use .pak files?
		if (PakCheckboxOption->CheckBox->IsChecked())
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

		// No debug info?
		if (StripDebugFilesCheckboxOption->CheckBox->IsChecked())
		{
			CookArgs += TEXT(" -nodebuginfo");
		}

		bool bWindows = WindowsCheckboxOption->CheckBox->IsChecked();
		bool bWindowsServer = WindowsServerCheckboxOption->CheckBox->IsChecked();
		bool bLinux = LinuxCheckboxOption->CheckBox->IsChecked();
		bool bLinuxServer = LinuxServerCheckboxOption->CheckBox->IsChecked();
		bool bStripDebug = StripDebugFilesCheckboxOption->CheckBox->IsChecked();
		bool bZipBuilds = ZipBuildCheckboxOption->CheckBox->IsChecked();
		bool bDeployToBuildManager = DeployToBuildManagerCheckboxOption->CheckBox->IsChecked();

		bool bRefreshBinariesOnly = RefreshBinariesOnlyCheckboxOption->CheckBox->IsChecked();

		Client->bUpdateBuildStatus = bZipBuilds && bDeployToBuildManager;

		if (bZipBuilds && bDeployToBuildManager)
		{
			Client->SetBuildManagerURL(BuildManagerUrlTextBox->GetText().ToString());
			
			class FGetNewBuildInfoTask : public FGenericHttpJsonTask
			{
			public:
				FGetNewBuildInfoTask(TSharedPtr<FLazyployLauncherClient> InClient, FString JsonBuildInfo)
					: FGenericHttpJsonTask(TEXT("GetBuildInfo"), TEXT("Get Build Info from Build Manager"), InClient->BuildManagerURL / TEXT("api/builds"), TEXT("POST"), JsonBuildInfo)
					, Client(InClient)
				{
				}
			protected:
				virtual bool PerformTask() override
				{
					if (FGenericHttpJsonTask::PerformTask() && JsonObj.IsValid())
					{	
						if (JsonObj->TryGetNumberField(TEXT("id"), Client->BuildId))
						{
							Client->SetBuildUploadEndpoint(Client->BuildManagerURL / TEXT("api/builds") / FString::FromInt(Client->BuildId) / TEXT("upload"));
							OnMessageRecieved().Broadcast(FString::Printf(TEXT("Successfully got new build info. Starting build with id: %d"), Client->BuildId));
							return true;
						}
					}

					OnMessageRecieved().Broadcast(TEXT("Failed to get build info from Build Manager"));
					return false;
				}
			private:
				TSharedPtr<FLazyployLauncherClient> Client;
			};

			TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
			JsonObj->SetStringField(TEXT("project"), Client->GetProjectName());
			JsonObj->SetStringField(TEXT("desc"), BuildDescriptionTextBox->GetText().ToString());
			JsonObj->SetStringField(TEXT("status"), TEXT("Started"));

			// Assemble platforms this build is targeting
			FString PlatformList;
			if (bWindows) PlatformList.Append(TEXT("WindowsNoEditor, "));
			if (bWindowsServer) PlatformList.Append(TEXT("WindowsServer, "));
			if (bLinux) PlatformList.Append(TEXT("LinuxNoEditor, "));
			if (bLinuxServer) PlatformList.Append(TEXT("LinuxServer, "));
			PlatformList = PlatformList.LeftChop(2);
			JsonObj->SetStringField(TEXT("platforms"), PlatformList);

			FString BuildInfoJson;
			const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&BuildInfoJson);
			FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter, true);

			CookProgress->AddTask(MakeShareable(new FGetNewBuildInfoTask(Client, BuildInfoJson)));
		}

		FString StagedBuildDir = Client->GetProjectDir() / TEXT("Saved/StagedBuilds");

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

			if (!bRefreshBinariesOnly)
			{
				CookProgress->NewTask(TEXT("WindowsCook"), TEXT("Cooking ") + TaskDesc, UATPath, CookArgs + PlatformString, Client->GetEngineBatchFilesPath());
			}
			

			// Set up Post Cook Tasks			
			if (bWindows)
			{
				const FString PlatformDir = TEXT("WindowsNoEditor");
				const FString BuildDir = StagedBuildDir / PlatformDir;
				
				if (bRefreshBinariesOnly)
				{
					if (bCodeProject)
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Win64");
						CookProgress->AddTask(MakeShareable(new FRefresCPPBinariesTask(ProjectBinaryDir, Client->GetProjectDir(), Client->GetProjectName(), TEXT("Win64"))));
					}
					else
					{
						const FString ProjectBinaryDir = BuildDir / TEXT("Engine") / TEXT("Binaries/Win64");
						CookProgress->AddTask(MakeShareable(new FRefreshBPBinariesTask(ProjectBinaryDir)));
					}
				}
				
				if (bStripDebug)
				{
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { get-childitem \"'%s'\" -include *.pdb -recurse | foreach ($_) {remove-item \"$_.fullname\"} }\""), *BuildDir);
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

				if (bRefreshBinariesOnly)
				{
					if (bCodeProject)
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Win64");
						CookProgress->AddTask(MakeShareable(new FRefresCPPBinariesTask(ProjectBinaryDir, Client->GetProjectDir(), Client->GetProjectName(), TEXT("Win64"), true)));
					}
					else
					{
						const FString ProjectBinaryDir = BuildDir / TEXT("Engine") / TEXT("Binaries/Win64");
						CookProgress->AddTask(MakeShareable(new FRefreshBPBinariesTask(ProjectBinaryDir, true)));
					}
				}

				if (bStripDebug)
				{
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { get-childitem \"'%s'\" -include *.pdb -recurse | foreach ($_) {remove-item $_.fullname} }\""), *BuildDir);
					CookProgress->NewTask(TEXT("StripDebugWindowsServer"), TEXT("Strip Debug Files for Windows Server"), TEXT("powershell.exe"), CommandArgs, TEXT(""));
				}

				if (!SteamSDK.IsEmpty() && WinServerSteamFixCheckboxOption->CheckBox->IsChecked())
				{
					FString SteamworksBinaryPath = FPaths::EngineDir() / TEXT("Binaries/ThirdParty/Steamworks") / SteamSDK / TEXT("Win64");
					FString StagedBinaryPath;
					if (bCodeProject)
					{
						StagedBinaryPath = BuildDir / Client->GetProjectName() / TEXT("Binaries/Win64");
					}
					else
					{
						StagedBinaryPath = BuildDir / TEXT("Engine/Binaries/Win64");
					}

					CookProgress->AddTask(MakeShareable(new FCopyDirTreeTask(StagedBinaryPath, SteamworksBinaryPath)));
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

			if (!bRefreshBinariesOnly)
			{
				CookProgress->NewTask(TEXT("LinuxCook"), TEXT("Cooking ") + TaskDesc, UATPath, CookArgs + PlatformString, Client->GetEngineBatchFilesPath());
			}
		
			// Set up Post Cook Tasks
			if (bLinux)
			{
				const FString PlatformDir = TEXT("LinuxNoEditor");
				const FString BuildDir = StagedBuildDir / PlatformDir;

				if (bRefreshBinariesOnly)
				{
					if (bCodeProject)
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Linux");
						CookProgress->AddTask(MakeShareable(new FRefresCPPBinariesTask(ProjectBinaryDir, Client->GetProjectDir(), Client->GetProjectName(), TEXT("Linux"))));
					}
					else
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Linux");
						CookProgress->AddTask(MakeShareable(new FRefreshBPBinariesTask(ProjectBinaryDir)));
					}
				}

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

				if (bRefreshBinariesOnly)
				{
					if (bCodeProject)
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Linux");
						CookProgress->AddTask(MakeShareable(new FRefresCPPBinariesTask(ProjectBinaryDir, Client->GetProjectDir(), Client->GetProjectName(), TEXT("Linux"), true)));
					}
					else
					{
						const FString ProjectBinaryDir = BuildDir / Client->GetProjectName() / TEXT("Binaries/Linux");
						CookProgress->AddTask(MakeShareable(new FRefreshBPBinariesTask(ProjectBinaryDir, true)));
					}
				}

				if (bStripDebug)
				{
					// @TODO: Strip Linux Debug Files
				}

				if (!SteamSDK.IsEmpty() && LinuxServerSteamFixCheckboxOption->CheckBox->IsChecked())
				{
					FString SteamworksBinaryPath = FPaths::EngineDir() / TEXT("Binaries/ThirdParty/Steamworks") / SteamSDK / TEXT("Linux");
					FString StagedBinaryPath;
					StagedBinaryPath = BuildDir / Client->GetProjectName() / TEXT("Binaries/Linux");
					CookProgress->AddTask(MakeShareable(new FCopyDirTreeTask(StagedBinaryPath, SteamworksBinaryPath)));
				}

				if (bZipBuilds)
				{
					static FString BuildArchivePath = FPaths::Combine(*StagedBuildDir, *(PlatformDir + TEXT(".zip")));
					FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*BuildArchivePath);
					FString CommandArgs = FString::Printf(TEXT("-nologo -noprofile -command \"& { Add-Type -A 'System.IO.Compression.FileSystem'; Try { [IO.Compression.ZipFile]::CreateFromDirectory('%s', '%s.zip'); } Catch { echo $_.Exception|format-list -force; exit 1; } }\""), *PlatformDir, *PlatformDir);
					CookProgress->NewTask(TEXT("ZipLinuxServer"), TEXT("Zip Linux Server Build"), TEXT("powershell.exe"), CommandArgs, StagedBuildDir);

					if (DeployToBuildManagerCheckboxOption->CheckBox->IsChecked())
					{
						CookProgress->NewTask(TEXT("UploadLinuxServer"), TEXT("Upload Linux Server Build"), Client->BuildUploadEndpoint, BuildArchivePath);
					}
				}
			}
		}

		CookProgress->ExecuteTasks();
	}
	
	return FReply::Handled();
}
