// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AllarBuilderClientApp.h"

#include "MainLoopTiming.h"
#include "LaunchEngineLoop.h"
#include "TaskGraphInterfaces.h"

FMainLoopTiming::FMainLoopTiming(float InIdealTickRate)
	: IdealFrameTime(1.f / InIdealTickRate)
{
}

void FMainLoopTiming::Tick()
{
	static double ActualDeltaTime = IdealFrameTime;
	static double LastTime = FPlatformTime::Seconds();

	// Tick app logic
	FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
	FTicker::GetCoreTicker().Tick(ActualDeltaTime);

	// Tick SlateApplication
	FSlateApplication::Get().PumpMessages();
	FSlateApplication::Get().Tick();

	// Sleep Throttling
	// Copied from Community Portal - should be shared
	FPlatformProcess::Sleep(FMath::Max<float>(0, IdealFrameTime - (FPlatformTime::Seconds() - LastTime)));

	// Calculate deltas
	const double AppTime = FPlatformTime::Seconds();
	ActualDeltaTime = AppTime - LastTime;
	LastTime = AppTime;
}
