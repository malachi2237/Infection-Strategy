// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InfectionStrategy : ModuleRules
{
	public InfectionStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "UMG" });
		PublicIncludePaths.AddRange(new string[] { "InfectionStrategy/Private/Actors", "InfectionStrategy/Private/Components", "InfectionStrategy/Private/Enums", "InfectionStrategy/Private/Interfaces", "InfectionStrategy/Private/Widgets" });
    }
}
