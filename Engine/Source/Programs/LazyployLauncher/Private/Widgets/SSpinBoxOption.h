// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "Widgets/Input/SSpinBox.h"

/**
* Implements a widget that is a int Spinbox with a label
*/
class SSpinBoxOption
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SSpinBoxOption)
		: _LabelText()
		, _MinValue(0)
		, _MaxValue(10)
		, _Value(0)
	{ }

	/** The text of the form label. */
	SLATE_ATTRIBUTE(FText, LabelText)

		/** The initial state of the checkbox */
		SLATE_ATTRIBUTE(ECheckBoxState, CheckboxState)

		/** Minimum value of SpinBox */
		SLATE_ATTRIBUTE(int32, MinValue)

		/** Max value of SpinBox */
		SLATE_ATTRIBUTE(int32, MaxValue)

		/** Default value of SpinBox */
		SLATE_ATTRIBUTE(int32, Value)

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
				SAssignNew(SpinBox, SSpinBox<int32>)
				.MinValue(InArgs._MinValue.Get())
				.MaxValue(InArgs._MaxValue.Get())
				.Value(InArgs._Value)
				.MinDesiredWidth(48.0f)
			]
		];
	}

public:

	// Holds the SpinBox
	TSharedPtr<SSpinBox<int32>> SpinBox;
};
