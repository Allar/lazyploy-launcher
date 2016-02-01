// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SCheckBoxOption.h"
#include "SSpinBoxOption.h"

/**
* Implements a widget for form input field labels with optional validation errors.
*/
class SClientLauncher
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SClientLauncher) { }
	SLATE_END_ARGS()

public:

	/**
	* Constructs the widget.
	*
	* @param InArgs The Slate argument list.
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle);

	bool IsAutoConnectEnabled() const;

	FReply LaunchClients();

private:

	TSharedPtr<FAllarBuilderClient> Client;

	TSharedPtr<SCheckboxOption> ListenServerCheckboxOption;
	TSharedPtr<SCheckboxOption> AutoConnectCheckboxOption;

	TSharedPtr<SSpinBoxOption> NumberOfClientsSpinBoxOption;
	TSharedPtr<SSpinBoxOption> ResXSpinBoxOption;
	TSharedPtr<SSpinBoxOption> ResYSpinBoxOption;
	TSharedPtr<SSpinBoxOption> PosXSpinBoxOption;
	TSharedPtr<SSpinBoxOption> PosYSpinBoxOption;

	TSharedPtr<SEditableTextBox> ClientArgumentsTextBox;
};
