// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BeyondTheFall : ModuleRules
{
	public BeyondTheFall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
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
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[]
		{
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
			"BeyondTheFall/Variant_SideScrolling/UI",
			Path.Combine(ModuleDirectory, "Public", "Gameplay", "Race")
		});
	}
}
