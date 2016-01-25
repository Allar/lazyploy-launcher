// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
* Implements the project loading area for the session launcher wizard.
*/
class SProjectPicker
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SProjectPicker) { }
	SLATE_END_ARGS()

public:

	/**
	* Destructor.
	*/
	~SProjectPicker();

public:

	/**
	* Constructs the widget.
	*
	* @param InArgs The Slate argument list.
	* @param InModel The data model.
	*/
	void Construct(const FArguments& InArgs, const FAllarBuilderClientRef& InModel);

protected:

	/**
	* Creates the widget for the project menu.
	*
	* @return The widget.
	*/
	TSharedRef<SWidget> MakeProjectMenuWidget();

	/**
	* Creates the widget for the project selection.
	*
	* @return The widget.
	*/
	TSharedRef<SWidget> MakeProjectWidget();

private:

	// Handles getting the text for the project combo button.
	FText HandleProjectComboButtonText() const;

	// Handles getting the tooltip for the project combo button.
	FText HandleProjectComboButtonToolTip() const;

	// Handles clicking a project menu entry.
	void HandleProjectMenuEntryClicked(FString ProjectPath);

	// Handles determining the visibility of a validation error icon.
	EVisibility HandleValidationErrorIconVisibility() const;

	// Sets the project in the appropriate place (profile if provided otherwise on the model)
	void SetProjectPath(FString ProjectPath);

private:

	// Holds the list of available projects.
	TArray<TSharedPtr<FString>> ProjectList;

	// Holds a pointer to the data model.
	FAllarBuilderClientPtr Model;

};
