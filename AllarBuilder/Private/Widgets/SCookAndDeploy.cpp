#include "../AllarBuilderClientApp.h"
#include "SClientLauncher.h"
#include "SCookProgress.h"
#include "SCookAndDeploy.h"

#define LOCTEXT_NAMESPACE "CookAndDeploy"

void SCookAndDeploy::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
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
			.Text(LOCTEXT("CookAndDeploy", "Cook"))
			.TextStyle(InStyle, TEXT("Section.Label"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(5.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(8.0f)
				[
					SNew(SVerticalBox)
					// Platforms
					+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(0.0f, 0.0f, 16.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Platforms", "Platforms:"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						// Windows
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Windows", "Windows"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// WindowsServer
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("WindowsServer", "WindowsServer"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// Linux
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Linux", "Linux"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// LinuxServer
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("LinuxServer", "LinuxServer"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
					]
					// Cook Options
					+ SVerticalBox::Slot()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(0.0f, 0.0f, 16.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Options", "Options:"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						// Use Pak files?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("UsePakFiles", "Use .pak"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// Compress?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Compress", "Compress?"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// Iterate?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Iterate", "Iterate?"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
						// Strip Version?
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(16.0f, 0.0f, 8.0f, 0.0f)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("StripVersion", "StripVersion"))
							.TextStyle(InStyle, TEXT("Section.Text"))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SCheckBox)
							.IsChecked(ECheckBoxState::Unchecked)
							//.OnCheckStateChanged(this, &SClientLauncher::OnListenServerChanged)
						]
					]
					// Begin Cook Button
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 8.0f, 0.0f, 0.0f)
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
		CookProgress->NewTask(TEXT("C:/Windows/System32/mspaint.exe"), TEXT(""), TEXT(""), TEXT("Test Paint Task"), TEXT("Testing to see how this works"), false);
		CookProgress->NewTask(TEXT("C:/Windows/System32/mspaint.exe"), TEXT(""), TEXT(""), TEXT("Second Task"), TEXT("Second test task"), false);
		CookProgress->ExecuteTasks();
	}
	
	return FReply::Handled();
}
