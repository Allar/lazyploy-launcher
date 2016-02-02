// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SCheckBox.h"

/**
* Implements a widget that is a check box with a label
*/
class SCheckboxOption
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCheckboxOption)
		: _LabelText()
		, _CheckboxState(ECheckBoxState::Unchecked)
		, _RightAlignCheckBox(false)
	{ }

	/** The text of the form label. */
	SLATE_ATTRIBUTE(FText, LabelText)

	/** The initial state of the checkbox */
	SLATE_ATTRIBUTE(ECheckBoxState, CheckboxState)

	/** Whether to right align the checkbox */
	SLATE_ATTRIBUTE(bool, RightAlignCheckBox)

	SLATE_END_ARGS()

public:

	/**
	* Constructs the widget.
	*
	* @param InArgs The Slate argument list.
	*/
	void Construct(const FArguments& InArgs, const TSharedRef<ISlateStyle>& InStyle)
	{
		ChildSlot
		[
			SNew(SHorizontalBox)
			+ (InArgs._RightAlignCheckBox.Get(false)
				? SHorizontalBox::Slot() // Right aligned
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				.FillWidth(1.0f)
				.Padding(0.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(InArgs._LabelText)
					.TextStyle(InStyle, TEXT("Section.Text"))
				]
				: SHorizontalBox::Slot() // Not right aligned
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				.AutoWidth()
				.Padding(0.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(InArgs._LabelText)
					.TextStyle(InStyle, TEXT("Section.Text"))
				]
			)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(InArgs._RightAlignCheckBox.Get(false) ? HAlign_Right : HAlign_Fill)
			.Padding(8.0f, 0.0f, 0.0f, 0.0f)
			[
				SAssignNew(CheckBox, SCheckBox)
				.IsChecked(InArgs._CheckboxState)
			]
		];
	}

public:

	// Holds the check box.
	TSharedPtr<SCheckBox> CheckBox;
};
