// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleUE4 : ModuleRules
{
	public SimpleUE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Preprocessor�� ���⿡ �ϱ�� ���� �����ϼž� �˴ϴ�.
            PublicDefinitions.Add("UNICODE");
            PublicDefinitions.Add("_UNICODE");

            // ������� Include ��θ� ���⿡ �ϱ�� ���� �����մϴ�.
            PublicIncludePaths.Add(@"E:\engine\dev003_3478\R2\ProudNet\include");

            // ���̺귯�� ���� ��θ� �ϱ�� ���� �����մϴ�.
            PublicAdditionalLibraries.Add(@"E:\engine\dev003_3478\R2\ProudNet\lib\x64\v140\Release\ProudNetClient.lib");
            PublicAdditionalLibraries.Add(@"E:\engine\dev003_3478\R2\ProudNet\Sample\bin\Release\SimpleCommon.lib");
        }
    }
}
