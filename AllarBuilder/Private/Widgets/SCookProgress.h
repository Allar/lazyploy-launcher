// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../GenericTask.h"

/**
* Implements a widget for form input field labels with optional validation errors.
*/
class SCookProgress
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCookProgress) { }
	SLATE_END_ARGS()

public:

	/**
	* Constructs the widget.
	*
	* @param InArgs The Slate argument list.
	*/
	void Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle);

	FText HandleProgressTextBlockText() const;
	// Callback for getting the filled percentage of the progress bar.
	TOptional<float> HandleProgressBarPercent() const;

	// Callback for generating a row in the task list view.
	TSharedRef<ITableRow> HandleTaskListViewGenerateRow(FGenericTaskPtr InItem, const TSharedRef<STableViewBase>& OwnerTable) const;

	bool IsReadyForNewJob() const;

	bool IsBusy() const;
	bool IsCancelling() const;
	bool IsPending() const;

	bool IsCancelButtonEnabled() const;
	FReply HandleCancelButtonClicked();

	void HandleTaskCompleted(const FString& CompletedTaskName);
	
	void ClearTasks();
	void AddTask(FGenericTaskPtr NewTask);
	void NewTask(const FString& InProcessPath, const FString& InProcessArguments, const FString& InWorkingDirectory, const FString& InName, const FString& InDesc, bool bInHidden = true);
	void ExecuteTasks();
	void CancelTasks();

protected:

private:

	TSharedPtr<FAllarBuilderClient> Client;

	// Holds the task list.
	TArray<FGenericTaskPtr> TaskList;

	// Widgets

	// Holds the progress bar.
	TSharedPtr<SProgressBar> ProgressBar;

	// Holds the task list view.
	TSharedPtr<SListView<FGenericTaskPtr>> TaskListView;

	// Holds the copy log button.
	TSharedPtr<SButton> CopyButton;

	// Holds the clear button.
	TSharedPtr<SButton> ClearButton;

	// Holds the save button.
	TSharedPtr<SButton> SaveButton;
};
