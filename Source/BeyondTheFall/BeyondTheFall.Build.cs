// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BeyondTheFall : ModuleRules
{
	public BeyondTheFall(ReadOnlyTargetRules Target) : base(Target)
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
			"SlateCore",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"BeyondTheFall",
			"BeyondTheFall/Variant_Platforming",
			"BeyondTheFall/Variant_Platforming/Animation",
			"BeyondTheFall/Variant_Combat",
			"BeyondTheFall/Variant_Combat/AI",
			"BeyondTheFall/Variant_Combat/Animation",
			"BeyondTheFall/Variant_Combat/Gameplay",
			"BeyondTheFall/Variant_Combat/Interfaces",
			"BeyondTheFall/Variant_Combat/UI",
			"BeyondTheFall/Variant_SideScrolling",
			"BeyondTheFall/Variant_SideScrolling/AI",
			"BeyondTheFall/Variant_SideScrolling/Gameplay",
			"BeyondTheFall/Variant_SideScrolling/Interfaces",
			"BeyondTheFall/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
