// Copyright 2016 Gamemakin LLC. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LazyployLauncherTarget : TargetRules
{
	public LazyployLauncherTarget(TargetInfo Target)
	{
		Type = TargetType.Program;
        AdditionalPlugins.Add("UdpMessaging");
    }

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutBuildBinaryConfigurations.Add(
			new UEBuildBinaryConfiguration(	InType: UEBuildBinaryType.Executable,
											InModuleNames: new List<string>() { "LazyployLauncher" } )
			);
	}

	public override bool ShouldCompileMonolithic(UnrealTargetPlatform InPlatform, UnrealTargetConfiguration InConfiguration)
	{
		return false;
	}

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

    public override bool GUBP_AlwaysBuildWithTools(UnrealTargetPlatform InHostPlatform, out bool bInternalToolOnly, out bool SeparateNode, out bool CrossCompile)
    {
        bInternalToolOnly = false;
        SeparateNode = false;
        CrossCompile = false;
        return true;
    }

    public override bool GUBP_NeedsPlatformSpecificDLLs()
    {
        return true;
    }
}
