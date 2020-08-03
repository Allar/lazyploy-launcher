// Copyright 2016 Gamemakin LLC. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LazyployLauncherTarget : TargetRules
{
	public LazyployLauncherTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Program;
		LaunchModuleName = "LazyployLauncher";
        AdditionalPlugins.Add("UdpMessaging");
		LinkType = TargetLinkType.Modular;
		
		bUseMallocProfiler = false;
		bBuildWithEditorOnlyData = true;
		
		bCompileAgainstEngine = false;
		bCompileAgainstCoreUObject = true;
        bForceBuildTargetPlatforms = true;
        bCompileWithStatsWithoutEngine = true;
        bCompileWithPluginSupport = true;

        bHasExports = false;
    }
}
