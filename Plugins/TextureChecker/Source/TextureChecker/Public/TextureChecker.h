// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Misc/AutomationTest.h"

//UENUM()
//enum TestAssetType
//{
	
	
//};

class FTextureCheckerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class FTextureCheckerHelper
{
public:
	static TArray<FString> ReadChangelist(FString FileName);
	static TArray<FString> ConvertGitPathsToUE(TArray<FString> Paths);
	static FString ConvertGitPathToUE(FString Path);
	static void IdentifyCategory(TArray<FString>& FilePath);

	static UTexture2D* GetTextureByPath(FString FilePath);
	static TArray<UTexture2D*> GetTexturesByPath(TArray<FString> FilePaths);
	
	static bool CheckTexture(const TArray<UTexture2D*>& Textures,
		TArray<FString>& OutErrors,
		TArray<FString>& OutInfos,
		int32& PassCount,
		int32& FailCount);
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FSimpleTest,
	"TextureChecker.SimpleTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
	)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTextureTest,
	"TextureChecker.TextureTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
	)

