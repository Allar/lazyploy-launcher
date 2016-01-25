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
						[
							SNew(SProjectPicker, Client)
						]
			]
		];
}

#undef LOCTEXT_NAMESPACE