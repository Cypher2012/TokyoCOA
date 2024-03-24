// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TokyoCOA : ModuleRules
{
	public TokyoCOA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"ProceduralMeshComponent", 
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
	}
}
