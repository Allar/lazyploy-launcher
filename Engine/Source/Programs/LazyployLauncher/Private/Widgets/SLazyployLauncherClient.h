// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "LazyployLauncherClient.h"

#include "SLazyployLauncherClient.h"
#include "LazyployLauncherClientStyle.h"
#include "SlateStyle.h"
#include "SThrobber.h"
#include "SNumericEntryBox.h"

#include "Widgets/SProjectPicker.h"
#include "Widgets/SClientLauncher.h"
#include "Widgets/SCookAndDeploy.h"

#define LOCTEXT_NAMESPACE "LazyployLauncherClient"

#if !CRASH_REPORT_UNATTENDED_ONLY

/**
* UI for the crash report client app
*/
class SLazyployLauncherClient : public SCompoundWidget
{
public:
	/**
	* Slate arguments
	*/
	SLATE_BEGIN_ARGS(SLazyployLauncherClient)
	{
	}

	SLATE_END_ARGS()

	/**
	* Construct this Slate ui
	* @param InArgs Slate arguments, not used
	* @param Client Allar Builder client implementation object
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FLazyployLauncherClient> Client, const TSharedRef<ISlateStyle>& InStyle)
	{
		LazyployLauncherClient = Client;

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
							.IsEnabled(Client, &FLazyployLauncherClient::IsProjectSelected)
							.OnClicked(Client, &FLazyployLauncherClient::LaunchEditor)
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
						.IsEnabled(Client, &FLazyployLauncherClient::IsProjectSelected)
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

private:

	/** Crash report client implementation object */
	TSharedPtr<FLazyployLauncherClient> LazyployLauncherClient;
};

#endif // !CRASH_REPORT_UNATTENDED_ONLY

#undef LOCTEXT_NAMESPACE
