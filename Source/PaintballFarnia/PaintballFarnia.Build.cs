// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PaintballFarnia : ModuleRules
{
	public PaintballFarnia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "PixelShader", "ComputeShader", "ProceduralMeshComponent", "RuntimeMeshComponent", "RHI", "RenderCore" });
	}
}
