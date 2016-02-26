#pragma once

#include "GenericTask.h"

class FRefresCPPBinariesTask : public FGenericTask
{
public:
	FRefresCPPBinariesTask(const FString InProjectBinaryDir, const FString InProjectDir, const FString InProjectName, const FString InTargetPlatform, bool bInServer = false)
	: FGenericTask(TEXT("Refresh") + InTargetPlatform + (bInServer ? TEXT("Server") : TEXT("")) + TEXT("CPPBinaries"), TEXT("Refresh ") + InTargetPlatform + (bInServer ? TEXT("Server") : TEXT("")) + TEXT(" Binaries"))
	, ProjectBinaryDir(InProjectBinaryDir)
	, ProjectDir(InProjectDir)
	, ProjectName(InProjectName)
	, TargetPlatform(InTargetPlatform)
	, bServer(bInServer)
	{
	}
protected:
	virtual bool PerformTask() override
	{
		if (bServer)
		{
			ProjectName += TEXT("Server");
		}

		bool bLinux = TargetPlatform.Contains(TEXT("Linux"));

		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*ProjectBinaryDir))
		{ 
			OnMessageRecieved().Broadcast(TEXT("Failed to delete project binary directory."));
			return false;
		}

		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ProjectBinaryDir);

		if (bLinux)
		{
			if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / ProjectName), *(ProjectDir / TEXT("Binaries") / TargetPlatform / ProjectName)))
			{
				OnMessageRecieved().Broadcast(TEXT("Failed to copy Project Binary"));
				return false;
			}
		}
		else
		{
			if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / (ProjectName + TEXT(".exe"))), *(ProjectDir / TEXT("Binaries") / TargetPlatform / (ProjectName + TEXT(".exe")))))
			{
				OnMessageRecieved().Broadcast(TEXT("Failed to copy Project Binary"));
				return false;
			}
			if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / (ProjectName + TEXT(".pdb"))), *(ProjectDir / TEXT("Binaries") / TargetPlatform / (ProjectName + TEXT(".pdb")))))
			{
				OnMessageRecieved().Broadcast(TEXT("Failed to copy Project Debug File"));
				return false;
			}
		}		
		return true;
	}
private:
	FString ProjectBinaryDir;
	FString ProjectDir;
	FString ProjectName;
	FString TargetPlatform;
	bool bServer;
};

class FRefreshBPBinariesTask : public FGenericTask
{
public:
	FRefreshBPBinariesTask(const FString InProjectBinaryDir, bool bInServer = false)
	: FGenericTask(FString::Printf(TEXT("Refresh%s%sBinaries"), InProjectBinaryDir.Contains(TEXT("Linux")) ? TEXT("Linux") : TEXT("Win64"), bInServer ? TEXT("Server") : TEXT("")), FString::Printf(TEXT("Refresh %s%s Binaries"), InProjectBinaryDir.Contains(TEXT("Linux")) ? TEXT("Linux") : TEXT("Win64"), bInServer ? TEXT("Server") : TEXT("")))
	, ProjectBinaryDir(InProjectBinaryDir)
	, bServer(bInServer)
	{
	}

protected:
	virtual bool PerformTask() override
	{
		const bool bLinux = ProjectBinaryDir.Contains(TEXT("Linux"));
		const FString BinaryName = bServer ? TEXT("UE4Server") : TEXT("UE4Game");
		FString ProjectPath = ProjectBinaryDir / TEXT("../..");
		FPaths::CollapseRelativeDirectories(ProjectPath);
		const FString ProjectName = FString::Printf(TEXT("%s%s"), *FPaths::GetBaseFilename(ProjectPath), bServer ? TEXT("Server") : TEXT(""));
		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*ProjectBinaryDir))
		{
			OnMessageRecieved().Broadcast(TEXT("Failed to delete project binary directory."));
			return false;
		}

		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ProjectBinaryDir);
		
		if (bLinux)
		{
			if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / ProjectName), *(FPaths::EngineDir() / TEXT("Binaries/Linux") / BinaryName)))
			{
				OnMessageRecieved().Broadcast(TEXT("Failed to copy Linux binary."));
				return false;
			}
		}
		else
		{
			if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / (BinaryName + TEXT(".exe"))), *(FPaths::EngineDir() / TEXT("Binaries/Win64") / (BinaryName + TEXT(".exe")))))
			{
				OnMessageRecieved().Broadcast(TEXT("Failed to copy Windows binary."));
				return false;
			}
		}
		
		return true;
	}
private:
	FString ProjectBinaryDir;
	bool bServer;
};