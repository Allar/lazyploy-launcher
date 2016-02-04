// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AllarBuilder : ModuleRules
{
	public AllarBuilder(TargetInfo Target)
	{
		PublicIncludePaths.Add("Runtime/Launch/Public");

		PrivateIncludePaths.Add("Runtime/Launch/Private");		// For LaunchEngineLoop.cpp include

		PrivateDependencyModuleNames.Add("Core");
		PrivateDependencyModuleNames.Add("Projects");

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "SlateReflector",
                "StandaloneRenderer",
                "MessageLog",
                "LauncherServices",
                "TargetPlatform",
                "SourceCodeAccess",
                "HTTP"
            }
        );
    }
}
