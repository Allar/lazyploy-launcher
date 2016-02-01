#include "../AllarBuilderClientApp.h"
#include "SClientLauncher.h"

#define LOCTEXT_NAMESPACE "ClientLauncher"

void SClientLauncher::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
{
	Client = InClient;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Clients", "Clients"))
			.TextStyle(InStyle, TEXT("Section.Label"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(5.0f)
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
			.Padding(8.0f)
			[
				SNew(SVerticalBox)
				// First row of Client options
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					// Number of Clients
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("NumberOfClients", "Number:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SSpinBox<uint32>)
						.MinValue(1)
						.MaxValue(4)
						.Value(1)
						.MinDesiredWidth(32.0f)
						.OnValueCommitted(this, &SClientLauncher::OnNumberOfClientsChanged)
						.OnValueChanged(this, &SClientLauncher::OnNumberOfClientsChanged, ETextCommit::Default)
						.OnEndSliderMovement(this, &SClientLauncher::OnNumberOfClientsChanged, ETextCommit::Default)
					]
					// Listen Server?
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ListenServer", "Listen?"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SCheckBox)
						.IsChecked(ECheckBoxState::Unchecked)
						.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
					]
					// Auto Connect?
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("AutoConnect", "Auto-Connect?"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SCheckBox)
						.IsChecked(ECheckBoxState::Unchecked)
						.IsEnabled(this, &SClientLauncher::IsAutoConnectEnabled)
						.OnCheckStateChanged(this, &SClientLauncher::OnAutoConnectChanged)
					]
					// Resolution X
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ResolutionX", "Res X:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SSpinBox<uint32>)
						.MinValue(320)
						.MaxValue(2560)
						.Value(1280)
						.MinDesiredWidth(48.0f)
						.OnValueCommitted(this, &SClientLauncher::OnResXChanged)
						.OnValueChanged(this, &SClientLauncher::OnResXChanged, ETextCommit::Default)
						.OnEndSliderMovement(this, &SClientLauncher::OnResXChanged, ETextCommit::Default)
					]
					// Resolution Y
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ResolutionY", "Res Y:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SSpinBox<uint32>)
						.MinValue(320)
						.MaxValue(1600)
						.Value(720)
						.MinDesiredWidth(48.0f)
						.OnValueCommitted(this, &SClientLauncher::OnResYChanged)
						.OnValueChanged(this, &SClientLauncher::OnResYChanged, ETextCommit::Default)
						.OnEndSliderMovement(this, &SClientLauncher::OnResYChanged, ETextCommit::Default)
					]
				]
				// Second row of Client options
				+ SVerticalBox::Slot()
				.Padding(0.0f, 8.0f, 0.0f, 0.0f)
				[
					SNew(SHorizontalBox)
					// Command line arguments
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("CommandLineArguments", "Arguments:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SEditableTextBox)
						.MinDesiredWidth(224)
						.OnTextCommitted(this, &SClientLauncher::OnClientArgumentsChanged)
						.OnTextChanged(this, &SClientLauncher::OnClientArgumentsChanged, ETextCommit::Default)
					]
					// Screen Position X
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PosX", "Pos X:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SSpinBox<uint32>)
						.MinValue(0)
						.MaxValue(3840)
						.Value(0)
						.MinDesiredWidth(48.0f)
						.OnValueCommitted(this, &SClientLauncher::OnPosXChanged)
						.OnValueChanged(this, &SClientLauncher::OnPosXChanged, ETextCommit::Default)
						.OnEndSliderMovement(this, &SClientLauncher::OnPosXChanged, ETextCommit::Default)
					]
					// Screen Position Y
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(16.0f, 0.0f, 8.0f, 0.0f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PosY", "Pos Y:"))
						.TextStyle(InStyle, TEXT("Section.Text"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SSpinBox<uint32>)
						.MinValue(0)
						.MaxValue(2160)
						.Value(0)
						.MinDesiredWidth(48.0f)
						.OnValueCommitted(this, &SClientLauncher::OnPosYChanged)
						.OnValueChanged(this, &SClientLauncher::OnPosYChanged, ETextCommit::Default)
						.OnEndSliderMovement(this, &SClientLauncher::OnPosYChanged, ETextCommit::Default)
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
	];
}

void SClientLauncher::OnNumberOfClientsChanged(uint32 Value, ETextCommit::Type CommitType)
{
	Client->NumberOfClients = Value;
}

void SClientLauncher::OnListenServerChanged(const ECheckBoxState NewCheckedState)
{
	Client->bLaunchListenServer = (NewCheckedState == ECheckBoxState::Checked);
}

void SClientLauncher::OnAutoConnectChanged(const ECheckBoxState NewCheckedState)
{
	Client->bAutoConnect = (NewCheckedState == ECheckBoxState::Checked);
}

void SClientLauncher::OnClientArgumentsChanged(const FText& InText, ETextCommit::Type InCommitType)
{
	Client->ClientArguments = InText.ToString();
}

void SClientLauncher::OnResXChanged(uint32 Value, ETextCommit::Type CommitType)
{
	Client->ResX = Value;
}

void SClientLauncher::OnResYChanged(uint32 Value, ETextCommit::Type CommitType)
{
	Client->ResY = Value;
}

void SClientLauncher::OnPosXChanged(uint32 Value, ETextCommit::Type CommitType)
{
	Client->PosX = Value;
}

void SClientLauncher::OnPosYChanged(uint32 Value, ETextCommit::Type CommitType)
{
	Client->PosY = Value;
}

bool SClientLauncher::IsAutoConnectEnabled() const
{
	return Client->bLaunchListenServer;
}

FReply SClientLauncher::LaunchClients()
{
	FString EditorBinaryPath = Client->GetEditorBinaryPath();
	uint32 ProcessID;

	FString NewClientArgs;
	for (uint32 i = 0; i < Client->NumberOfClients; ++i)
	{
		// First (host?) client
		if (i == 0)
		{
			NewClientArgs = Client->ClientArguments;
			if (Client->bLaunchListenServer)
			{
				NewClientArgs = (NewClientArgs.Len() == 0) ? TEXT("-listen") : NewClientArgs + TEXT("?Listen");
			}
		}
		// Secondary (client?) clients
		else
		{
			// Ignore provided arguments if launching listen server
			if (Client->bLaunchListenServer)
			{
				// AutoConnect will connect to local listen server
				if (Client->bAutoConnect)
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
				NewClientArgs = Client->ClientArguments;
			}
		}

		FString ResAndPosArgs = FString::Printf(TEXT(" -resx=%d -resy=%d -winx=%d -winy=%d"), Client->ResX, Client->ResY, Client->PosX + ((i % 2) * Client->ResX), Client->PosY + ((i / 2) * Client->ResY));

		NewClientArgs = FString::Printf(TEXT("%s %s -game %s"), *Client->GetProjectPath(), *NewClientArgs, *ResAndPosArgs);

		UE_LOG(LogAllarBuilder, Log, TEXT("Launching client with arguments: %s"), *NewClientArgs);

		FPlatformProcess::CreateProc(*EditorBinaryPath, *NewClientArgs, true, false, false, &ProcessID, 0, nullptr, nullptr);
	}
	return FReply::Handled();
}