#include "../LazyployLauncherClientApp.h"
#include "SCheckboxOption.h"
#include "SClientLauncher.h"

#define LOCTEXT_NAMESPACE "ClientLauncher"

void SClientLauncher::Construct(const FArguments& InArgs, TSharedRef<FLazyployLauncherClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
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
				.Text(LOCTEXT("Clients", "Clients"))
				.TextStyle(InStyle, TEXT("Section.Label"))
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(5.0f)
		[
			SNew(SBorder)
			.HAlign(HAlign_Left)
			.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
			.Padding(8.0f)
			[
				SNew(SBox)
				.MaxDesiredWidth(650)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(0.0f, 0.0f, 8.0f, 0.0f)
						[
							SAssignNew(NumberOfClientsSpinBoxOption, SSpinBoxOption, InStyle)
							.LabelText(LOCTEXT("NumberOfClients", "Number:"))
							.MinValue(1)
							.MaxValue(4)
							.Value(1)
						]
						// Listen Server?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 8.0f, 0.0f)
						[
							SAssignNew(ListenServerCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("ListenServer", "Listen"))
						]
						// Auto Connect?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 8.0f, 0.0f)
						[
							SAssignNew(AutoConnectCheckboxOption, SCheckboxOption, InStyle)
							.LabelText(LOCTEXT("Join", "Join"))
							.IsEnabled(this, &SClientLauncher::IsAutoConnectEnabled)
						
						]
						// Resolution X
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 8.0f, 0.0f)
						[
							SAssignNew(ResXSpinBoxOption, SSpinBoxOption, InStyle)
							.LabelText(LOCTEXT("ResolutionX", "Res X:"))
							.MinValue(320)
							.MaxValue(2560)
							.Value(1280)
						
						]
						// Resolution Y
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 0.0f, 0.0f)
						[
							SAssignNew(ResYSpinBoxOption, SSpinBoxOption, InStyle)
							.LabelText(LOCTEXT("ResolutionY", "Res Y:"))
							.MinValue(320)
							.MaxValue(1600)
							.Value(720)
						]
					]
					+ SVerticalBox::Slot()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(3.0f)
						.Padding(0.0f, 0.0f, 40.0f, 0.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Left)
							.AutoWidth()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("CommandLineArguments", "Arguments:"))
								.TextStyle(InStyle, TEXT("Section.Text"))
							]
							+ SHorizontalBox::Slot()
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Fill)
							.FillWidth(1.0f)
							.Padding(8.0f, 0.0, 0.0f, 0.0f)
							[
								SAssignNew(ClientArgumentsTextBox, SEditableTextBox)
								.MinDesiredWidth(600)
							]
						]
						// Screen Position X
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 8.0f, 0.0f)
						[
							SAssignNew(PosXSpinBoxOption, SSpinBoxOption, InStyle)
							.LabelText(LOCTEXT("PosX", "Pos X:"))
							.MinValue(0)
							.MaxValue(3840)
							.Value(0)
						]
						// Screen Position Y
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Fill)
						.FillWidth(1.0f)
						.Padding(8.0f, 0.0f, 0.0f, 0.0f)
						[
							SAssignNew(PosYSpinBoxOption, SSpinBoxOption, InStyle)
							.LabelText(LOCTEXT("PosY", "Pos Y:"))
							.MinValue(0)
							.MaxValue(2160)
							.Value(0)
						]
					]
					+ SVerticalBox::Slot()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						SNew(SButton)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.OnClicked(this, &SClientLauncher::LaunchClients)
						.Content()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("LaunchClients", "Launch Client(s)"))
						]
					]
				]
			]
		]
	];

	LoadOptionsFromConfig();
}

// Probably the worst way to do this.
// @TODO: Make a data storage class
void SClientLauncher::LoadOptionsFromConfig()
{
	// Default Client Options
	bool bListenServerEnabled = false;
	bool bAutoConnectEnabled = false;
	int32 NumClients = 1;
	int32 ResX = 1280;
	int32 ResY = 720;
	int32 PosX = 0;
	int32 PosY = 0;
	FText ClientArgs = FText::GetEmpty();

	// Load Default Client Options
	GConfig->GetBool(TEXT("Lazyploy.Clients"), TEXT("bListenServer"), bListenServerEnabled, GEngineIni);
	GConfig->GetBool(TEXT("Lazyploy.Clients"), TEXT("bAutoConnect"), bAutoConnectEnabled, GEngineIni);
	GConfig->GetInt(TEXT("Lazyploy.Clients"), TEXT("NumClients"), NumClients, GEngineIni);
	GConfig->GetInt(TEXT("Lazyploy.Clients"), TEXT("ResX"), ResX, GEngineIni);
	GConfig->GetInt(TEXT("Lazyploy.Clients"), TEXT("ResY"), ResY, GEngineIni);
	GConfig->GetInt(TEXT("Lazyploy.Clients"), TEXT("PosX"), PosX, GEngineIni);
	GConfig->GetInt(TEXT("Lazyploy.Clients"), TEXT("PosY"), PosY, GEngineIni);
	GConfig->GetText(TEXT("Lazyploy.Clients"), TEXT("Args"), ClientArgs, GEngineIni);

// @HACK: SetIsChecked would not take a bool. Is there a better way?
#define BOOL_TO_CHECKED(inBool) inBool ? ECheckBoxState::Checked : ECheckBoxState::Unchecked

	ListenServerCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bListenServerEnabled));
	AutoConnectCheckboxOption->CheckBox->SetIsChecked(BOOL_TO_CHECKED(bAutoConnectEnabled));
	NumberOfClientsSpinBoxOption->SpinBox->SetValue(NumClients);
	ResXSpinBoxOption->SpinBox->SetValue(ResX);
	ResYSpinBoxOption->SpinBox->SetValue(ResY);
	PosXSpinBoxOption->SpinBox->SetValue(PosX);
	PosYSpinBoxOption->SpinBox->SetValue(PosY);
	ClientArgumentsTextBox->SetText(ClientArgs);

#undef BOOL_TO_CHECKED

}

void SClientLauncher::SaveOptionsToConfig()
{
	GConfig->SetBool(TEXT("Lazyploy.Clients"), TEXT("bListenServer"), ListenServerCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetBool(TEXT("Lazyploy.Clients"), TEXT("bAutoConnect"), AutoConnectCheckboxOption->CheckBox->IsChecked(), GEngineIni);
	GConfig->SetInt(TEXT("Lazyploy.Clients"), TEXT("NumClients"), NumberOfClientsSpinBoxOption->SpinBox->GetValue(), GEngineIni);
	GConfig->SetInt(TEXT("Lazyploy.Clients"), TEXT("ResX"), ResXSpinBoxOption->SpinBox->GetValue(), GEngineIni);
	GConfig->SetInt(TEXT("Lazyploy.Clients"), TEXT("ResY"), ResYSpinBoxOption->SpinBox->GetValue(), GEngineIni);
	GConfig->SetInt(TEXT("Lazyploy.Clients"), TEXT("PosX"), PosXSpinBoxOption->SpinBox->GetValue(), GEngineIni);
	GConfig->SetInt(TEXT("Lazyploy.Clients"), TEXT("PosY"), PosYSpinBoxOption->SpinBox->GetValue(), GEngineIni);
	GConfig->SetText(TEXT("Lazyploy.Clients"), TEXT("Args"), ClientArgumentsTextBox->GetText(), GEngineIni);	

	GConfig->Flush(false, GEngineIni);
}

bool SClientLauncher::IsAutoConnectEnabled() const
{
	return ListenServerCheckboxOption->CheckBox->IsChecked();
}

FReply SClientLauncher::LaunchClients()
{
	SaveOptionsToConfig();

	FString EditorBinaryPath = Client->GetEditorBinaryPath();
	uint32 ProcessID;

	FString NewClientArgs;
	for (int32 i = 0; i < (int32)NumberOfClientsSpinBoxOption->SpinBox->GetValue(); ++i)
	{
		// First (host?) client
		if (i == 0)
		{
			NewClientArgs = ClientArgumentsTextBox->GetText().ToString();
			if (ListenServerCheckboxOption->CheckBox->IsChecked())
			{
				NewClientArgs = (NewClientArgs.Len() == 0) ? TEXT("-listen") : NewClientArgs + TEXT("?Listen");
			}
		}
		// Secondary (client?) clients
		else
		{
			// Ignore provided arguments if launching listen server
			if (ListenServerCheckboxOption->CheckBox->IsChecked())
			{
				// AutoConnect will connect to local listen server
				if (AutoConnectCheckboxOption->CheckBox->IsChecked())
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
				NewClientArgs = ClientArgumentsTextBox->GetText().ToString();
			}
		}

		FString ResAndPosArgs = FString::Printf(TEXT(" -resx=%d -resy=%d -winx=%d -winy=%d"), (int32)ResXSpinBoxOption->SpinBox->GetValue(), (int32)ResYSpinBoxOption->SpinBox->GetValue(), (int32)PosXSpinBoxOption->SpinBox->GetValue() + ((i % 2) * (int32)ResXSpinBoxOption->SpinBox->GetValue()), (int32)PosYSpinBoxOption->SpinBox->GetValue() + ((i / 2) * (int32)ResYSpinBoxOption->SpinBox->GetValue()));

		NewClientArgs = FString::Printf(TEXT("%s %s -game %s"), *Client->GetProjectPath(), *NewClientArgs, *ResAndPosArgs);

		UE_LOG(LogLazyployLauncher, Log, TEXT("Launching client with arguments: %s"), *NewClientArgs);

		FPlatformProcess::CreateProc(*EditorBinaryPath, *NewClientArgs, true, false, false, &ProcessID, 0, nullptr, nullptr);
	}
	return FReply::Handled();
}