#include "AllarBuilderClientApp.h"

#include "SAllarBuilderClient.h"
#include "AllarBuilderClientStyle.h"
#include "SlateStyle.h"
#include "SThrobber.h"
#include "SNumericEntryBox.h"

#include "Widgets/SProjectPicker.h"
#include "Widgets/SClientLauncher.h"
#include "Widgets/SCookAndDeploy.h"

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
						SNew(SClientLauncher, Client, InStyle)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SSeparator)
						.Orientation(Orient_Horizontal)
					]
					// Cook and Deploy
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(8.0f)
					[
						SNew(SCookAndDeploy, Client, InStyle)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SSeparator)
						.Orientation(Orient_Horizontal)
					]
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE