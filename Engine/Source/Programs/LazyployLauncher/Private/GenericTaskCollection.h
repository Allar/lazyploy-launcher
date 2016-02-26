#pragma once

#include "GenericTask.h"

class FRefreshWindowsProjectCPPBinaries : public FGenericTask
{
public:
	FRefreshWindowsProjectCPPBinaries(const FString InProjectBinaryDir, const FString InProjectDir, const FString InProjectName, const FString InTargetPlatform, bool bInServer = false)
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

		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*ProjectBinaryDir))
		{ 
			OnMessageRecieved().Broadcast(TEXT("Failed to delete project binary directory."));
			return false;
		}

		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ProjectBinaryDir);

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
		return true;
	}
private:
	FString ProjectBinaryDir;
	FString ProjectDir;
	FString ProjectName;
	FString TargetPlatform;
	bool bServer;
};

class FRefreshWindowsBPBinaries : public FGenericTask
{
public:
	FRefreshWindowsBPBinaries(const FString InProjectBinaryDir, bool bInServer = false)
	: FGenericTask(bInServer ? TEXT("RefreshWin64ServerBPBinaries") : TEXT("RefreshWin64BPBinaries"), bInServer ? TEXT("Refresh Win64Server Binaries") : TEXT("Refresh Win64 Binaries"))
	, ProjectBinaryDir(InProjectBinaryDir)
	, bServer(bInServer)
	{
	}

protected:
	virtual bool PerformTask() override
	{
		const FString BinaryName = bServer ? TEXT("UE4Server") : TEXT("UE4Game");
		if (!FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*ProjectBinaryDir))
		{
			OnMessageRecieved().Broadcast(TEXT("Failed to delete project binary directory."));
			return false;
		}

		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ProjectBinaryDir);
		
		if (!FPlatformFileManager::Get().GetPlatformFile().CopyFile(*(ProjectBinaryDir / (BinaryName + TEXT(".exe"))), *(FPaths::EngineDir() / TEXT("Binaries/Win64") / (BinaryName + TEXT(".exe")))))
		{
			OnMessageRecieved().Broadcast(TEXT("Failed to copy UE4Game.exe")); 
			return false; 
		}
		return true;
	}
private:
	FString ProjectBinaryDir;
	bool bServer;
};