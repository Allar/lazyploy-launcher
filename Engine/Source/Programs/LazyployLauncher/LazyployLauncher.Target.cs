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
    }

	//
	// TargetRules interface.
	//

	public override void SetupGlobalEnvironment(
		TargetInfo Target,
		ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
		ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration
		)
	{
		// Lean and mean
		//UEBuildConfiguration.bCompileLeanAndMeanUE = true;

		// Never use malloc profiling in Unreal Header Tool.  We set this because often UHT is compiled right before the engine
		// automatically by Unreal Build Tool, but if bUseMallocProfiler is defined, UHT can operate incorrectly.
		BuildConfiguration.bUseMallocProfiler = false;

		// No editor needed
		UEBuildConfiguration.bBuildEditor = false;
		// Editor-only data, however, is needed
		UEBuildConfiguration.bBuildWithEditorOnlyData = true;

		// Currently this app is not linking against the engine, so we'll compile out references from Core to the rest of the engine
		UEBuildConfiguration.bCompileAgainstEngine = false;
		UEBuildConfiguration.bCompileAgainstCoreUObject = true;
        UEBuildConfiguration.bForceBuildTargetPlatforms = true;
        UEBuildConfiguration.bCompileWithStatsWithoutEngine = true;
        UEBuildConfiguration.bCompileWithPluginSupport = true;

        OutLinkEnvironmentConfiguration.bHasExports = false;
    }
}
