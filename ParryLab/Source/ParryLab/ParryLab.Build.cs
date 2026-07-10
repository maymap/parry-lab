// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ParryLab : ModuleRules
{
	public ParryLab(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ParryLab",
			"ParryLab/Variant_Platforming",
			"ParryLab/Variant_Platforming/Animation",
			"ParryLab/Variant_Combat",
			"ParryLab/Variant_Combat/AI",
			"ParryLab/Variant_Combat/Animation",
			"ParryLab/Variant_Combat/Gameplay",
			"ParryLab/Variant_Combat/Interfaces",
			"ParryLab/Variant_Combat/UI",
			"ParryLab/Variant_SideScrolling",
			"ParryLab/Variant_SideScrolling/AI",
			"ParryLab/Variant_SideScrolling/Gameplay",
			"ParryLab/Variant_SideScrolling/Interfaces",
			"ParryLab/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
