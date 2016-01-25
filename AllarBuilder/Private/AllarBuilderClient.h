// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AllarBuilderClientApp.h"

/** Type definition for shared pointers to instances of FAllarBuilderClient. */
typedef TSharedPtr<class FAllarBuilderClient> FAllarBuilderClientPtr;

/** Type definition for shared references to instances of FAllarBuilderClient. */
typedef TSharedRef<class FAllarBuilderClient> FAllarBuilderClientRef;

/**
* Main implementation of the Allar Builder client application
*/
class FAllarBuilderClient : public TSharedFromThis<FAllarBuilderClient>
{

public:
	/**
	* Constructor:
	*
	*/
	FAllarBuilderClient();

	/** Destructor. */
	virtual ~FAllarBuilderClient();

	/**
	* Handle user closing the main window
	* @param Window Main window
	*/
	void RequestCloseWindow(const TSharedRef<SWindow>& Window);

	FString GetEditorBinaryPath() const;

	// Project options
	FString GetProjectPath() const;
	void SetProjectPath(const FString NewProjectPath);
	FString GetProjectName() const;

	bool IsProjectSelected() const;
	FReply LaunchEditor();

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

	// Project settings
	FString ProjectPath;

	// Client settings
	uint32 NumberOfClients;
	bool bLaunchListenServer;
	bool bAutoConnect;

	uint32 ResX;
	uint32 ResY;
	uint32 PosX;
	uint32 PosY;

	FString ClientArguments;
	

};