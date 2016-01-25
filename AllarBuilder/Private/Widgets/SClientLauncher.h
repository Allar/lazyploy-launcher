// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

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

	// Client options
	void OnNumberOfClientsChanged(uint32 Value, ETextCommit::Type CommitType);
	void OnListenServerChanged(const ECheckBoxState NewCheckedState);
	void OnAutoConnectChanged(const ECheckBoxState NewCheckedState);
	void OnClientArgumentsChanged(const FText& InText, ETextCommit::Type InCommitType);
	void OnResXChanged(uint32 Value, ETextCommit::Type CommitType);
	void OnResYChanged(uint32 Value, ETextCommit::Type CommitType);
	void OnPosXChanged(uint32 Value, ETextCommit::Type CommitType);
	void OnPosYChanged(uint32 Value, ETextCommit::Type CommitType);

	bool IsAutoConnectEnabled() const;

	FReply LaunchClients();

private:

	TSharedPtr<FAllarBuilderClient> Client;
};
