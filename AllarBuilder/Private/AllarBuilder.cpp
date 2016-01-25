// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "AllarBuilderClientApp.h"
#include "AllarBuilder.h"

#include "AllarBuilderClient.h"
#include "SAllarBuilderClient.h"
#include "AllarBuilderClientStyle.h"
#include "ISlateReflectorModule.h"

#include "MainLoopTiming.h"

#include "RequiredProgramMainCPPInclude.h"

DEFINE_LOG_CATEGORY_STATIC(LogAllarBuilder, Log, All);

IMPLEMENT_APPLICATION(AllarBuilder, "AllarBuilder");

/** Average tick rate the app aims for */
const float IdealTickRate = 30.f;

/** Default main window size */
const FVector2D InitialWindowDimensions(740, 560);

void RunAllarBuilderClient(const TCHAR* CommandLine)
{
	GEngineLoop.PreInit(CommandLine);

	UE_LOG(LogAllarBuilder, Display, TEXT("Hello World"));

	// Set up the main ticker
	FMainLoopTiming MainLoop(IdealTickRate);

	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());

	// Prepare the custom Slate styles
	FAllarBuilderClientStyle::Initialize();

	// Create the main implementation object
	TSharedRef<FAllarBuilderClient> AllarBuilderClient = MakeShareable(new FAllarBuilderClient());

	// open up the app window	
	TSharedRef<SAllarBuilderClient> ClientControl = SNew(SAllarBuilderClient, AllarBuilderClient);

	auto Window = FSlateApplication::Get().AddWindow(
		SNew(SWindow)
		.Title(NSLOCTEXT("AllarBuilder", "AllarBuilderClientAppName", "Allar Builder"))
		.ClientSize(InitialWindowDimensions)
		[
			ClientControl
		]
	);

	// Setting focus seems to have to happen after the Window has been added
	FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);

	// loop until the app is ready to quit
	while (!GIsRequestingExit)
	{
		MainLoop.Tick();
	}

	Window->SetRequestDestroyWindowOverride(FRequestDestroyWindowOverride::CreateSP(AllarBuilderClient, &FAllarBuilderClient::RequestCloseWindow));

	// Clean up the custom styles
	FAllarBuilderClientStyle::Shutdown();

	// Close down the Slate application
	FSlateApplication::Shutdown();

	FEngineLoop::AppPreExit();
	FTaskGraphInterface::Shutdown();

	FEngineLoop::AppExit();
}