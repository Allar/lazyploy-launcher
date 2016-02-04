#pragma once

class FGenericProcessTask
	: public FGenericTask
{
public:
	FGenericProcessTask(const FString& InName, const FString& InDesc, const FString& InProcessPath, const FString& InProcessArguments, const FString& InWorkingDirectory, bool bInHidden = true)
		: FGenericTask(InName, InDesc) 
		, ProcessPath(InProcessPath)
		, ProcessArguments(InProcessArguments)
		, WorkingDirectory(InWorkingDirectory)
		, bHidden(bInHidden)
		, ProcessHandle(nullptr)
		, ReadPipe(nullptr)
		, WritePipe(nullptr)
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
	* Do the work of this task.
	*
	* @return - State data of the task chain.
	*/
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

protected:

	// Process information
	FString ProcessPath;
	FString ProcessArguments;
	FString WorkingDirectory;
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

	FProcHandle ProcessHandle;

	// read and write pipe
	void* ReadPipe;
	void* WritePipe;

	// result
	int32 Result;


};