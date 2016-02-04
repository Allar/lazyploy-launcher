#pragma once

/** Type definition for shared pointers to instances of FGenericTask. */
typedef TSharedPtr<class FGenericTask> FGenericTaskPtr;

/** Type definition for shared references to instances of FGenericTask. */
typedef TSharedRef<class FGenericTask> FGenericTaskRef;

class FGenericTask
	: public FRunnable
	, public ILauncherTask
{
public:
	FGenericTask( const FString& InName, const FString& InDesc )
		: Name(InName)
		, Desc(InDesc)
		, Result(0)
		, Status(ELauncherTaskStatus::Pending)
		, Thread(nullptr)
	{
	}

public:

	/**
	* Executes the tasks.
	*
	* @param ChainState - State data of the task chain.
	*/
	virtual void Execute()
	{
		check(Status == ELauncherTaskStatus::Pending);

		Thread = FRunnableThread::Create(this, TEXT("FLauncherTask"));

		if (Thread == nullptr)
		{
			// @TODO: Indicate that we failed to create a thread
		}
	}

	/**
	* Do the work of this task.
	*
	* @return - State data of the task chain.
	*/
	virtual bool PerformTask() = 0;

	// Begin FRunnable interface
public:
	virtual bool Init() override
	{
		return true;
	}

	virtual uint32 Run() override
	{
		Status = ELauncherTaskStatus::Busy;

		StartTime = FDateTime::UtcNow();

		TaskStarted.Broadcast(Name);
		if (PerformTask())
		{
			Status = ELauncherTaskStatus::Completed;

			TaskCompleted.Broadcast(Name);
		}
		else
		{
			if (Status == ELauncherTaskStatus::Canceling)
			{
				Status = ELauncherTaskStatus::Canceled;
			}
			else
			{
				Status = ELauncherTaskStatus::Failed;
			}

			TaskCompleted.Broadcast(Name);
		}

		EndTime = FDateTime::UtcNow();

		return 0;
	}

	virtual void Stop() override
	{
		Cancel();
	}

	virtual void Exit() override { }

	// End FRunnable interface

	// Begin ILauncherTask interface
public:

	virtual void Cancel() override
	{
		if (Status == ELauncherTaskStatus::Busy)
		{
			Status = ELauncherTaskStatus::Canceling;
		}
		else if (Status == ELauncherTaskStatus::Pending)
		{
			Status = ELauncherTaskStatus::Canceled;
		}
	}

	virtual FTimespan GetDuration() const override
	{
		if (Status == ELauncherTaskStatus::Pending)
		{
			return FTimespan::Zero();
		}

		if (Status == ELauncherTaskStatus::Busy)
		{
			return (FDateTime::UtcNow() - StartTime);
		}

		return (EndTime - StartTime);
	}

	virtual const FString& GetName() const override
	{
		return Name;
	}

	virtual const FString& GetDesc() const override
	{
		return Desc;
	}

	virtual ELauncherTaskStatus::Type GetStatus() const override
	{
		return Status;
	}

	virtual bool IsFinished() const override
	{
		return ((Status == ELauncherTaskStatus::Canceled) ||
			(Status == ELauncherTaskStatus::Completed) ||
			(Status == ELauncherTaskStatus::Failed));
	}

	virtual FOnTaskStartedDelegate& OnStarted() override
	{
		return TaskStarted;
	}

	virtual FOnTaskCompletedDelegate& OnCompleted() override
	{
		return TaskCompleted;
	}

	// End ILauncherTask interface

	virtual FOutputMessageReceivedDelegate& OnMessageRecieved()
	{
		return MessageRecieved;
	}

	int32 ReturnCode() const override
	{
		return Result;
	}

protected:
	
	// Task information
	FString Name;
	FString Desc;
	FDateTime StartTime;
	FDateTime EndTime;
	int32 Result;

	// Holds the status of this task
	ELauncherTaskStatus::Type Status;

	// Holds the thread that's running this task.
	FRunnableThread* Thread;

	// Message delegates
	FOnTaskStartedDelegate TaskStarted;
	FOnTaskCompletedDelegate TaskCompleted;
	FOutputMessageReceivedDelegate MessageRecieved;

};