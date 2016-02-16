// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#include "../LazyployLauncherClientApp.h"
#include "STaskListRow.h"
#include "SThrobber.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
TSharedRef<SWidget> STaskListRow::GenerateWidgetForColumn(const FName& ColumnName)
{
	if (ColumnName == "Duration")
	{
		return SNew(SBox)
			.Padding(FMargin(4.0, 0.0))
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &STaskListRow::HandleDurationText)
			];
	}
	else if (ColumnName == "Icon")
	{
		return SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SThrobber)
				.Animate(SThrobber::VerticalAndOpacity)
			.NumPieces(1)
			.Visibility(this, &STaskListRow::HandleThrobberVisibility)
			]

		+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.ColorAndOpacity(this, &STaskListRow::HandleIconColorAndOpacity)
			.Image(this, &STaskListRow::HandleIconImage)
			];
	}
	else if (ColumnName == "Status")
	{
		return SNew(SBox)
			.Padding(FMargin(4.0, 0.0))
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &STaskListRow::HandleStatusText)
			];
	}
	else if (ColumnName == "Task")
	{
		ILauncherTaskPtr TaskPtr = Task.Pin();

		if (TaskPtr.IsValid())
		{
			return SNew(SBox)
				.Padding(FMargin(4.0, 0.0))
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TaskPtr->GetDesc()))
				];
		}
	}

	return SNullWidget::NullWidget;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
