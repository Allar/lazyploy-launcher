// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once

#include "LazyployLauncherClientApp.h"

/** Type definition for shared pointers to instances of FLazyployLauncherClient. */
typedef TSharedPtr<class FLazyployLauncherClient> FLazyployLauncherClientPtr;

/** Type definition for shared references to instances of FLazyployLauncherClient. */
typedef TSharedRef<class FLazyployLauncherClient> FLazyployLauncherClientRef;

/**
* Main implementation of the Allar Builder client application
*/
class FLazyployLauncherClient : public TSharedFromThis<FLazyployLauncherClient>
{

public:
	/**
	* Constructor:
	*
	*/
	FLazyployLauncherClient();

	/** Destructor. */
	virtual ~FLazyployLauncherClient();

	/**
	* Handle user closing the main window
	* @param Window Main window
	*/
	void RequestCloseWindow(const TSharedRef<SWindow>& Window);

	FString GetEditorBinaryPath() const;
	FString GetEngineBatchFilesPath() const;

	// Project options
	FString GetProjectPath() const;
	void SetProjectPath(const FString NewProjectPath);
	FString GetProjectDir() const;
	FString GetProjectName() const;

	void SetBuildUploadEndpoint(FString NewEndpoint);
	void SetBuildManagerURL(FString NewURL);

	bool IsProjectSelected() const;
	FReply LaunchEditor();

public:

	// Project settings
	FString ProjectPath;

	// Current build info for Build Manager
	int32 BuildId;
	FString& BuildUploadEndpoint;
	FString& BuildManagerURL;

	bool bUpdateBuildStatus;
};