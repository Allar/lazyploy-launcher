// Copyright 2016 Gamemakin LLC. All Rights Reserved.

using UnrealBuildTool;

public class LazyployLauncher : ModuleRules
{
	public LazyployLauncher(TargetInfo Target)
	{
		PublicIncludePaths.Add("Runtime/Launch/Public");

		PrivateIncludePaths.Add("Runtime/Launch/Private");		// For LaunchEngineLoop.cpp include

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AutomationController",
                "Core",
                "CoreUObject",
                "Projects",
                "DeviceManager",
                "Slate",
                "SlateCore",
                "SlateReflector",
                "StandaloneRenderer",
                "Messaging",
                "Profiler",
                "ProfilerClient",
                "SessionFrontend",
                "SessionServices",
                "MessageLog",
                "LauncherServices",
                "TargetPlatform",
                "TargetDeviceServices",
                "SourceCodeAccess",
                "HTTP",
                "JSON",
                "InputCore",
                "Networking",
                "Sockets",
                "UdpMessaging"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PrivateDependencyModuleNames.Add("XCodeSourceCodeAccess");
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDependencyModuleNames.Add("VisualStudioSourceCodeAccess");
        }
    }
}
