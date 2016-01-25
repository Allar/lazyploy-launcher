#include "AllarBuilderClientApp.h"

#include "SAllarBuilderClient.h"
#include "AllarBuilderClientStyle.h"
#include "SlateStyle.h"
#include "SThrobber.h"
#include "SNumericEntryBox.h"

#include "Widgets/SProjectPicker.h"

#define LOCTEXT_NAMESPACE "AllarBuilderClient"

void SAllarBuilderClient::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> Client, const TSharedRef<ISlateStyle>& InStyle)
{
	AllarBuilderClient = Client;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(InStyle->GetBrush(TEXT("Background")))
		]
		+ SOverlay::Slot()
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			.Padding(8.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(8.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SProjectPicker, Client)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Right)
					[
						SNew(SButton)
						.VAlign(VAlign_Center)
						.IsEnabled(Client, &FAllarBuilderClient::IsProjectSelected)
						.OnClicked(Client, &FAllarBuilderClient::LaunchEditor)
						.Content()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("LaunchEditor", "Editor"))
							.TextStyle(InStyle, TEXT("ProjectPicker.Text"))
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SVerticalBox)
					.IsEnabled(Client, &FAllarBuilderClient::IsProjectSelected)
					// Clients
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(8.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						.Padding(0.0f, 0.0f, 16.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Clients", "Clients:"))
							.TextStyle(InStyle, TEXT("Section.Label"))
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
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
										.OnValueCommitted(Client, &FAllarBuilderClient::OnNumberOfClientsChanged)
										.OnValueChanged(Client, &FAllarBuilderClient::OnNumberOfClientsChanged, ETextCommit::Default)
										.OnEndSliderMovement(Client, &FAllarBuilderClient::OnNumberOfClientsChanged, ETextCommit::Default)
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
										.OnCheckStateChanged(Client, &FAllarBuilderClient::OnListenServerChanged)
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
										.IsEnabled(Client, &FAllarBuilderClient::IsAutoConnectEnabled)
										.OnCheckStateChanged(Client, &FAllarBuilderClient::OnAutoConnectChanged)
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
										.OnValueCommitted(Client, &FAllarBuilderClient::OnResXChanged)
										.OnValueChanged(Client, &FAllarBuilderClient::OnResXChanged, ETextCommit::Default)
										.OnEndSliderMovement(Client, &FAllarBuilderClient::OnResXChanged, ETextCommit::Default)
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
										.OnValueCommitted(Client, &FAllarBuilderClient::OnResYChanged)
										.OnValueChanged(Client, &FAllarBuilderClient::OnResYChanged, ETextCommit::Default)
										.OnEndSliderMovement(Client, &FAllarBuilderClient::OnResYChanged, ETextCommit::Default)
									]
								]
								// Second row of Client options
								+ SVerticalBox::Slot()
								.Padding(0.0f, 8.0f, 0.0f, 0.0f)
								[
									SNew(SHorizontalBox)
									// Command line arguments
									+SHorizontalBox::Slot()
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
										.OnTextCommitted(Client, &FAllarBuilderClient::OnClientArgumentsChanged)
										.OnTextChanged(Client, &FAllarBuilderClient::OnClientArgumentsChanged, ETextCommit::Default)
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
										.OnValueCommitted(Client, &FAllarBuilderClient::OnPosXChanged)
										.OnValueChanged(Client, &FAllarBuilderClient::OnPosXChanged, ETextCommit::Default)
										.OnEndSliderMovement(Client, &FAllarBuilderClient::OnPosXChanged, ETextCommit::Default)
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
										.OnValueCommitted(Client, &FAllarBuilderClient::OnPosYChanged)
										.OnValueChanged(Client, &FAllarBuilderClient::OnPosYChanged, ETextCommit::Default)
										.OnEndSliderMovement(Client, &FAllarBuilderClient::OnPosYChanged, ETextCommit::Default)
									]
								]
								+ SVerticalBox::Slot()
								.Padding(0.0f, 8.0f, 0.0f, 0.0f)
								[
									SNew(SButton)
									.VAlign(VAlign_Center)
									.HAlign(HAlign_Center)
									.OnClicked(Client, &FAllarBuilderClient::LaunchClients)
									.Content()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("LaunchClients", "Launch Client(s)"))
									]
								]
							]
						]
					]
					
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE