// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AllarBuilderClientApp.h"

/**
* WinMain, called when the application is started
*/
int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR, _In_ int nCmdShow)
{
	// Run the app
	RunAllarBuilderClient(GetCommandLineW());

	return 0;
}
