// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once
#include "../GenericTask.h"
#include "SGenericTaskMessageListRow.h"

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
	void Construct(const FArguments& InArgs, TSharedRef<FLazyployLauncherClient> InClient, const TSharedRef<ISlateStyle>& InStyle);

	FText HandleProgressTextBlockText() const;
	// Callback for getting the filled percentage of the progress bar.
	TOptional<float> HandleProgressBarPercent() const;

	// Callback for generating a row in the task list view.
	TSharedRef<ITableRow> HandleTaskListViewGenerateRow(FGenericTaskPtr InItem, const TSharedRef<STableViewBase>& OwnerTable) const;

	// Callback for generating a row in the message/log output window.
	TSharedRef<ITableRow> HandleMessageListViewGenerateRow(TSharedPtr<FGenericTaskMessage> InItem, const TSharedRef<STableViewBase>& OwnerTable) const;

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	bool IsReadyForNewJob() const;

	bool IsBusy() const;
	bool IsCancelling() const;
	bool IsPending() const;
	bool Failed() const;

	bool IsCancelButtonEnabled() const;
	FReply HandleCancelButtonClicked();

	void HandleTaskMessageReceived(const FString& InMessage);
	void HandleTaskCompleted(const FString& CompletedTaskName);
	
	void ClearTasks();
	void AddTask(FGenericTaskPtr NewTask);
	void NewTask(const FString& InName, const FString& InDesc, const FString& InProcessPath, const FString& InProcessArguments, const FString& InWorkingDirectory, bool bInHidden = true);
	void NewUploadTask(const FString& InName, const FString& InDesc, const FString InPlatform, const FString& InFilePath);
	void ExecuteTasks();
	void CancelTasks();

	void UpdateBuildManagerStatus(const FString& BuildStatus);

	bool IsClearLogEnabled() const;
	FReply HandleClearLogButtonClicked();
	void ClearLog();

protected:

private:

	TSharedPtr<FLazyployLauncherClient> Client;

	// Holds the task list.
	TArray<FGenericTaskPtr> TaskList;

	// Widgets

	// Holds the progress bar.
	TSharedPtr<SProgressBar> ProgressBar;

	// Holds the task list view.
	TSharedPtr<SListView<FGenericTaskPtr>> TaskListView;

	// Holds the message list.
	TArray< TSharedPtr<FGenericTaskMessage>> MessageList;

	// Holds the pending message list.
	TArray< TSharedPtr<FGenericTaskMessage>> PendingMessages;

	// Holds the message list view.
	TSharedPtr<SListView<TSharedPtr<FGenericTaskMessage>>> MessageListView;

	// Critical section for updating the messages
	FCriticalSection CriticalSection;

	// Holds the clear button.
	TSharedPtr<SButton> ClearButton;
};
