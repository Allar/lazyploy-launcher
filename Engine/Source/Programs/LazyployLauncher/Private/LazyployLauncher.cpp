// Copyright 2016 Gamemakin LLC. All Rights Reserved.


#include "LazyployLauncher.h"

#include "LazyployLauncherClient.h"
#include "Widgets/SLazyployLauncherClient.h"
#include "LazyployLauncherClientStyle.h"
#include "SDockTab.h"

#include "RequiredProgramMainCPPInclude.h"

#include "AutomationController.h"
#include "ISourceCodeAccessModule.h"

DEFINE_LOG_CATEGORY_STATIC(LogLazyployLauncher, Log, All);

IMPLEMENT_APPLICATION(LazyployLauncher, "LazyployLauncher");

#define IDEAL_FRAMERATE 60;

namespace LazyployLauncher
{
	TSharedPtr<FTabManager::FLayout> ApplicationLayout;
}

TSharedPtr<FLazyployLauncherClient> LazyployLauncherClient = nullptr;

TSharedRef<SDockTab> SpawnLazyployLauncherTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const TSharedRef<SDockTab> DockTab = SNew(SDockTab)
		.TabRole(ETabRole::MajorTab);

	DockTab->SetContent(SNew(SLazyployLauncherClient, LazyployLauncherClient.ToSharedRef(), FLazyployLauncherClientStyle::GetPtr().ToSharedRef()));

	return DockTab;
}

void RunLazyployLauncherClient(const TCHAR* CommandLine)
{
	FString NewCommandLine = CommandLine;

	if (!FParse::Param(*NewCommandLine, TEXT("-Messaging")))
	{
		NewCommandLine += TEXT(" -Messaging");
	}

	if (!FParse::Param(*NewCommandLine, TEXT("-http")))
	{
		NewCommandLine += TEXT(" -http=wininet");
	}

	GEngineLoop.PreInit(*NewCommandLine);
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();

	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());

	FString LazyployFrontendLayoutIni = FPaths::GetPath(GEngineIni) + "/Layout.ini";
	// load required modules
	FModuleManager::Get().LoadModuleChecked("EditorStyle");
	FModuleManager::Get().LoadModuleChecked("Messaging");

	IAutomationControllerModule& AutomationControllerModule = FModuleManager::LoadModuleChecked<IAutomationControllerModule>("AutomationController");
	AutomationControllerModule.Init();

	// load plug-ins
	IPluginManager::Get().LoadModulesForEnabledPlugins(ELoadingPhase::PreDefault);

	FModuleManager::Get().LoadModule("ProfilerClient");
	FModuleManager::Get().LoadModule("ProjectLauncher");
	FModuleManager::Get().LoadModule("SessionFrontend");

	FGlobalTabmanager::Get()->SetApplicationTitle(NSLOCTEXT("LazyployLauncher", "AppTitle", "Lazyploy Launcher"));

	// Prepare the custom Slate styles
	FLazyployLauncherClientStyle::Initialize();

	// Create the main implementation object
	LazyployLauncherClient = MakeShareable(new FLazyployLauncherClient());

	// Register tab
	auto& TabSpawnerEntry = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("LazyployLauncher"), FOnSpawnTab::CreateStatic(&SpawnLazyployLauncherTab))
		.SetDisplayName(NSLOCTEXT("LazyployLauncher", "TabTitle", "Lazyploy Launcher"))
		.SetTooltipText(NSLOCTEXT("LazyployLauncher", "TabTooltipText", "Open the Lazyploy Launcher tab."));

	// restore application layout
	TSharedRef<FTabManager::FLayout> NewLayout = FTabManager::NewLayout("LazyployLauncherLayout_v1.0")
		->AddArea
		(
			FTabManager::NewArea(800.f, 800.0f)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FName("SessionFrontend"), ETabState::OpenedTab)
				->AddTab(FName("LazyployLauncher"), ETabState::OpenedTab)
			)
		);

	LazyployLauncher::ApplicationLayout = FLayoutSaveRestore::LoadFromConfig(LazyployFrontendLayoutIni, NewLayout);
	FGlobalTabmanager::Get()->RestoreFrom(LazyployLauncher::ApplicationLayout.ToSharedRef(), TSharedPtr<SWindow>());

	// enter main loop
	double DeltaTime = 0.0;
	double LastTime = FPlatformTime::Seconds();
	const float IdealFrameTime = 1.0f / IDEAL_FRAMERATE;

	// loop until the app is ready to quit
	while (!GIsRequestingExit)
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();
		FTicker::GetCoreTicker().Tick(DeltaTime);

		// throttle frame rate
		FPlatformProcess::Sleep(FMath::Max<float>(0.0f, IdealFrameTime - (FPlatformTime::Seconds() - LastTime)));

		double CurrentTime = FPlatformTime::Seconds();
		DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;

		FStats::AdvanceFrame(false);

		GLog->FlushThreadedLogs();
	}


	// Clean up the custom styles
	FLazyployLauncherClientStyle::Shutdown();

	// save application layout
	FLayoutSaveRestore::SaveToConfig(LazyployFrontendLayoutIni, LazyployLauncher::ApplicationLayout.ToSharedRef());
	GConfig->Flush(false, LazyployFrontendLayoutIni);

	// shut down application
	FSlateApplication::Shutdown();

	// shut down
	FEngineLoop::AppPreExit();
	FModuleManager::Get().UnloadModulesAtShutdown();

#if STATS
	FThreadStats::StopThread();
#endif

	FTaskGraphInterface::Shutdown();
}