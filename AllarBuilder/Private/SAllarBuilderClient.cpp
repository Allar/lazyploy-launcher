#include "AllarBuilderClientApp.h"

#include "SAllarBuilderClient.h"
#include "AllarBuilderClientStyle.h"
#include "SlateStyle.h"
#include "SThrobber.h"

#include "Widgets/SProjectPicker.h"

#define LOCTEXT_NAMESPACE "AllarBuilderClient"

void SAllarBuilderClient::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> Client)
{
	AllarBuilderClient = Client;

	ChildSlot
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
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
											.IsEnabled(AllarBuilderClient.Get(), &FAllarBuilderClient::IsEditorEnabled)
											.OnClicked(AllarBuilderClient.Get(), &FAllarBuilderClient::LaunchEditor)
											.Content()
											[
												SNew(STextBlock)
													.Text(LOCTEXT("LaunchEditor", "Editor"))
													.TextStyle(FAllarBuilderClientStyle::Get(), TEXT("ProjectPicker.Text"))
											]
									]
						]
						
			]
		];
}

#undef LOCTEXT_NAMESPACE