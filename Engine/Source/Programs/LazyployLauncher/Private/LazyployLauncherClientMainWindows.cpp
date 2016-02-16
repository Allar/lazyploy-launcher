// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#include "LazyployLauncherClientApp.h"

/**
* WinMain, called when the application is started
*/
int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR, _In_ int nCmdShow)
{
	// Run the app
	RunLazyployLauncherClient(GetCommandLineW());

	return 0;
}
