// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KuroTask_AutoTest : ModuleRules
{
	public KuroTask_AutoTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
