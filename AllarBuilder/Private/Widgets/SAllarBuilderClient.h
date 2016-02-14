// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AllarBuilderClient.h"

#include "SAllarBuilderClient.h"
#include "AllarBuilderClientStyle.h"
#include "SlateStyle.h"
#include "SThrobber.h"
#include "SNumericEntryBox.h"

#include "Widgets/SProjectPicker.h"
#include "Widgets/SClientLauncher.h"
#include "Widgets/SCookAndDeploy.h"

#define LOCTEXT_NAMESPACE "AllarBuilderClient"

#if !CRASH_REPORT_UNATTENDED_ONLY

/**
* UI for the crash report client app
*/
class SAllarBuilderClient : public SCompoundWidget
{
public:
	/**
	* Slate arguments
	*/
	SLATE_BEGIN_ARGS(SAllarBuilderClient)
	{
	}

	SLATE_END_ARGS()

	/**
	* Construct this Slate ui
	* @param InArgs Slate arguments, not used
	* @param Client Allar Builder client implementation object
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> Client, const TSharedRef<ISlateStyle>& InStyle)
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

private:

	/** Crash report client implementation object */
	TSharedPtr<FAllarBuilderClient> AllarBuilderClient;
};

#endif // !CRASH_REPORT_UNATTENDED_ONLY

#undef LOCTEXT_NAMESPACE
