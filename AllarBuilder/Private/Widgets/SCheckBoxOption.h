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
	{ }

	/** The text of the form label. */
	SLATE_ATTRIBUTE(FText, LabelText)

	/** The initial state of the checkbox */
	SLATE_ATTRIBUTE(ECheckBoxState, CheckboxState)

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
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.AutoWidth()
			.Padding(0.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(InArgs._LabelText)
				.TextStyle(InStyle, TEXT("Section.Text"))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Fill)
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
