#include "../AllarBuilderClientApp.h"
#include "SCookProgress.h"
#include "../STaskListRow.h"
#include "../GenericTask.h"
#include "SGenericTaskMessageListRow.h"

#define LOCTEXT_NAMESPACE "CookProgress"

void SCookProgress::Construct(const FArguments& InArgs, TSharedRef<FAllarBuilderClient> InClient, const TSharedRef<ISlateStyle>& InStyle)
{
	Client = InClient;

	ChildSlot
	[
		SNew(SVerticalBox)
		//+ SVerticalBox::Slot()
		//.AutoHeight()
		//[
		//	SNew(SBorder)
		//	.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		//	.Padding(8.0f)
		//	[
		//		SNew(SVerticalBox)
		//		+ SVerticalBox::Slot()
		//		.AutoHeight()
		//		[
		//			SNew(STextBlock)
		//			.Text(this, &SCookProgress::HandleProgressTextBlockText)
		//		]

		//		+ SVerticalBox::Slot()
		//		.AutoHeight()
		//		.Padding(0.0f, 4.0f, 0.0f, 0.0f)
		//		[
		//			SAssignNew(ProgressBar, SProgressBar)
		//			.Percent(this, &SCookProgress::HandleProgressBarPercent)
		//		]
		//	]
		//]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[
			SNew(SBox)
			.HeightOverride(80)
			[
			
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(0.0f)
				[
					SAssignNew(TaskListView, SListView<FGenericTaskPtr>)
					.HeaderRow
					(
						SNew(SHeaderRow)

						+ SHeaderRow::Column("Icon")
						.DefaultLabel(LOCTEXT("TaskListIconColumnHeader", " "))
						.FixedWidth(20.0)

						+ SHeaderRow::Column("Task")
						.DefaultLabel(LOCTEXT("TaskListTaskColumnHeader", "Task"))
						.FillWidth(1.0)

						+ SHeaderRow::Column("Duration")
						.DefaultLabel(LOCTEXT("TaskListDurationColumnHeader", "Duration"))
						.FixedWidth(64.0)

						+ SHeaderRow::Column("Status")
						.DefaultLabel(LOCTEXT("TaskListStatusColumnHeader", "Status"))
						.FixedWidth(80.0)
					)
					.ListItemsSource(&TaskList)
					.OnGenerateRow(this, &SCookProgress::HandleTaskListViewGenerateRow)
					.ItemHeight(24.0)
					.SelectionMode(ESelectionMode::Single)
				]
			]
		]

		//content area for the log
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[
			SNew(SBox)
			.HeightOverride(150)
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
				.Padding(0.0f)
				[
					SAssignNew(MessageListView, SListView< TSharedPtr<FGenericTaskMessage> >)
					.HeaderRow
					(
						SNew(SHeaderRow)
						+ SHeaderRow::Column("Status")
						.DefaultLabel(LOCTEXT("TaskListStatusColumnHeader", "Output Log"))
						.FillWidth(1.0)
					)
					.ListItemsSource(&MessageList)
					.OnGenerateRow(this, &SCookProgress::HandleMessageListViewGenerateRow)
					.ItemHeight(24.0)
					.SelectionMode(ESelectionMode::Multi)
				]
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 5.0f, 0.0f, 0.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				// clear button
				SAssignNew(ClearButton, SButton)
				.ContentPadding(FMargin(6.0f, 2.0f))
				.Text(LOCTEXT("ClearButtonText", "Clear Log"))
				.ToolTipText(LOCTEXT("ClearButtonTooltip", "Clear the log window"))
				.IsEnabled(this, &SCookProgress::IsClearLogEnabled)
				.OnClicked(this, &SCookProgress::HandleClearLogButtonClicked)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				// Cancel button
				SNew(SButton)
				.ContentPadding(FMargin(6.0f, 2.0f))
				.IsEnabled(this, &SCookProgress::IsCancelButtonEnabled)
				.OnClicked(this, &SCookProgress::HandleCancelButtonClicked)
				//.ToolTipText(this, &SProjectLauncherProgress::GetDoneButtonToolTip)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("CancelButtonText", "Cancel"))
				]
			]
		]
	];
}

FText SCookProgress::HandleProgressTextBlockText() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("NumCompleted"), 0);
	Args.Add(TEXT("NumFailed"), 0);
	Args.Add(TEXT("NumCanceled"), 0);

	return FText::Format(LOCTEXT("TasksFinishedFormatText", "Operation finished. Completed: {NumCompleted}, Failed: {NumFailed}, Canceled: {NumCanceled}"), Args);
}

TOptional<float> SCookProgress::HandleProgressBarPercent() const
{
	return 0.0f;
}

TSharedRef<ITableRow> SCookProgress::HandleTaskListViewGenerateRow(FGenericTaskPtr InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(STaskListRow)
		.Task(InItem)
		.OwnerTableView(OwnerTable);
}

TSharedRef<ITableRow> SCookProgress::HandleMessageListViewGenerateRow(TSharedPtr<FGenericTaskMessage> InItem, const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(SGenericTaskMessageListRow, OwnerTable)
		.Message(InItem)
		.ToolTipText(InItem->Message);
}

void SCookProgress::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (PendingMessages.Num() > 0)
	{
		FScopeLock ScopedLock(&CriticalSection);
		for (int32 Index = 0; Index < PendingMessages.Num(); ++Index)
		{
			MessageList.Add(PendingMessages[Index]);
		}
		PendingMessages.Reset();
		MessageListView->RequestListRefresh();
		MessageListView->RequestScrollIntoView(MessageList.Last());
	}
}

bool SCookProgress::IsReadyForNewJob() const
{
	return !(IsBusy() || IsCancelling() || IsPending());
}

bool SCookProgress::IsBusy() const
{
	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		if (TaskList[i]->GetStatus() == ELauncherTaskStatus::Busy)
		{
			return true;
		}
	}
	return false;
}

bool SCookProgress::IsCancelling() const
{
	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		if (TaskList[i]->GetStatus() == ELauncherTaskStatus::Canceling)
		{
			return true;
		}
	}
	return false;
}

bool SCookProgress::IsPending() const
{
	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		if (TaskList[i]->GetStatus() == ELauncherTaskStatus::Pending)
		{
			return true;
		}
	}
	return false;
}

bool SCookProgress::IsCancelButtonEnabled() const
{
	return IsBusy() && !IsCancelling();
}

FReply SCookProgress::HandleCancelButtonClicked()
{
	CancelTasks();

	return FReply::Handled();
}

void SCookProgress::HandleTaskMessageReceived(const FString& InMessage)
{
	FScopeLock ScopedLock(&CriticalSection);
	ELogVerbosity::Type Verbosity = ELogVerbosity::Log;

	if (InMessage.Contains(TEXT("Automation.ParseCommandLine:"), ESearchCase::CaseSensitive))
	{
		Verbosity = ELogVerbosity::Display;
	}
	else if (InMessage.Contains(TEXT("Error:"), ESearchCase::IgnoreCase))
	{
		Verbosity = ELogVerbosity::Error;
	}
	else if (InMessage.Contains(TEXT("Warning:"), ESearchCase::IgnoreCase))
	{
		Verbosity = ELogVerbosity::Warning;
	}
	TSharedPtr<FGenericTaskMessage> Message = MakeShareable(new FGenericTaskMessage(FText::FromString(InMessage), Verbosity));
	PendingMessages.Add(Message);
}

void SCookProgress::HandleTaskCompleted(const FString& CompletedTaskName)
{
	int32 CompletedTaskIndex = -1;

	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		if (TaskList[i]->GetName() == CompletedTaskName)
		{
			CompletedTaskIndex = i;
			break;
		}
	}

	if (CompletedTaskIndex != -1 && (CompletedTaskIndex + 1) < TaskList.Num())
	{
		if (TaskList[CompletedTaskIndex]->GetStatus() == ELauncherTaskStatus::Completed)
		{
			FPlatformProcess::Sleep(0.25);
			TaskList[CompletedTaskIndex + 1]->Execute();
		}
		else
		{
			HandleCancelButtonClicked();
		}
	}
}

void SCookProgress::ClearTasks()
{
	if (IsReadyForNewJob())
	{
		TaskList.Empty();
		TaskListView->RequestListRefresh();
	}
}

void SCookProgress::AddTask(FGenericTaskPtr NewTask)
{
	NewTask->OnCompleted().AddRaw(this, &SCookProgress::HandleTaskCompleted);
	NewTask->OnMessageRecieved().AddRaw(this, &SCookProgress::HandleTaskMessageReceived);
	TaskList.Add(NewTask);
	TaskListView->RequestListRefresh();
}

void SCookProgress::NewTask(const FString& InProcessPath, const FString& InProcessArguments, const FString& InWorkingDirectory, const FString& InName, const FString& InDesc, bool bInHidden /*= true*/)
{
	FGenericTaskPtr NewTask = MakeShareable(new FGenericTask(InProcessPath, InProcessArguments, InWorkingDirectory, InName, InDesc, bInHidden));
	NewTask->OnCompleted().AddRaw(this, &SCookProgress::HandleTaskCompleted);
	NewTask->OnMessageRecieved().AddRaw(this, &SCookProgress::HandleTaskMessageReceived);
	TaskList.Add(NewTask);
	TaskListView->RequestListRefresh();
}

void SCookProgress::ExecuteTasks()
{
	// If we're busy already, do nothing
	if (IsBusy() || IsCancelling())
	{
		return;
	}

	ClearLog();

	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		if (TaskList[i]->GetStatus() == ELauncherTaskStatus::Pending)
		{
			TaskList[i]->Execute();
			return;
		}
	}
	TaskListView->RequestListRefresh();
}

void SCookProgress::CancelTasks()
{
	for (int32 i = 0; i < TaskList.Num(); ++i)
	{
		TaskList[i]->Cancel();
	}
}

bool SCookProgress::IsClearLogEnabled() const
{
	return MessageList.Num() > 0;
}

FReply SCookProgress::HandleClearLogButtonClicked()
{
	ClearLog();
	return FReply::Handled();
}

void SCookProgress::ClearLog()
{
	MessageList.Reset();
	MessageListView->RequestListRefresh();
}
