#include "../AllarBuilderClientApp.h"
#include "SClientLauncher.h"

#define LOCTEXT_NAMESPACE "ClientLauncher"

void SCookAndDeploy::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
{
	Client = InClient;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.VAlign(VAlign_Center)
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
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
			.Padding(8.0f)
			[
				SNew(SVerticalBox)
				// First row of Client options
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
				]
			]
		]
	];
}