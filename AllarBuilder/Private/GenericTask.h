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
	FGenericTask( const FString& InProcessPath, const FString& InProcessArguments, const FString& InWorkingDirectory, const FString& InName, const FString& InDesc, bool bInHidden = true)
		: ProcessPath(InProcessPath)
		, ProcessArguments(InProcessArguments)
		, WorkingDirectory(InWorkingDirectory)
		, ProcessHandle(nullptr)
		, Name(InName)
		, Desc(InDesc)
		, ReadPipe(nullptr)
		, WritePipe(nullptr)
		, bHidden(bInHidden)
		, Result(0)
		, Status(ELauncherTaskStatus::Pending)
		, Thread(nullptr)
	{
	}

public:

	// Read in any output from the process and broadcast each line using OutputMessageRecieved
	virtual void ReadAllProcessOutput()
	{
		FString NewLine = FPlatformProcess::ReadPipe(ReadPipe);
		while (NewLine.Len() > 0)
		{
			// process the string to break it up in to lines
			TArray<FString> StringArray;
			int32 Count = NewLine.ParseIntoArray(StringArray, TEXT("\n"), true);

			for (int32 Index = 0; Index < Count; ++Index)
			{
				StringArray[Index].TrimTrailing();
				if (StringArray[Index].Len() > 0)
				{
					OnMessageRecieved().Broadcast(StringArray[Index]);
				}
			}

			NewLine = FPlatformProcess::ReadPipe(ReadPipe);
		}
	}

	/**
	* Executes the tasks.
	*
	* @param ChainState - State data of the task chain.
	*/
	void Execute()
	{
		check(Status == ELauncherTaskStatus::Pending);

		Thread = FRunnableThread::Create(this, TEXT("FLauncherTask"));

		if (Thread == nullptr)
		{
			// @TODO: Indicate that we failed to create a thread
		}
	}

	virtual bool PerformTask()
	{
		FString Line; // Holds last line of output from process

		FPlatformProcess::CreatePipe(ReadPipe, WritePipe);

		WorkingDirectory = FPaths::ConvertRelativePathToFull(WorkingDirectory);
		FPaths::MakePlatformFilename(WorkingDirectory);

		// Log this task's process
		FString TaskStartDesc = FString::Printf(TEXT("Task starting up process: %s %s in working directory %s"), *ProcessPath, *ProcessArguments, *WorkingDirectory);
		OnMessageRecieved().Broadcast(TaskStartDesc);
		
		// Start process
		ProcessHandle = FPlatformProcess::CreateProc(*ProcessPath, *ProcessArguments, false, bHidden, bHidden, NULL, 0, WorkingDirectory.Len() > 0 ? *WorkingDirectory : nullptr, WritePipe);

		// Monitor process
		while (FPlatformProcess::IsProcRunning(ProcessHandle))
		{
			ReadAllProcessOutput();

			if (GetStatus() == ELauncherTaskStatus::Canceling)
			{
				FPlatformProcess::TerminateProc(ProcessHandle, true);
				return false;
			}

			FPlatformProcess::Sleep(0.25);
		}

		ReadAllProcessOutput();

		FPlatformProcess::ClosePipe(ReadPipe, WritePipe);

		if (!FPlatformProcess::GetProcReturnCode(ProcessHandle, &Result))
		{
			return false;
		}

		return (Result == 0);
	}

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

private:
	
	// Process information
	FString ProcessPath;
	FString ProcessArguments;
	FString WorkingDirectory;
	FProcHandle ProcessHandle;
	bool bHidden;

	// Task information
	FString Name;
	FString Desc;
	FDateTime StartTime;
	FDateTime EndTime;

	// Holds the status of this task
	ELauncherTaskStatus::Type Status;

	// Holds the thread that's running this task.
	FRunnableThread* Thread;

	// Message delegates
	FOnTaskStartedDelegate TaskStarted;
	FOnTaskCompletedDelegate TaskCompleted;
	FOutputMessageReceivedDelegate MessageRecieved;

protected:

	// read and write pipe
	void* ReadPipe;
	void* WritePipe;

	// result
	int32 Result;


};