// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Misc/AutomationTest.h"

//UENUM()
//enum TestAssetType
//{
	
	
//};

enum class ETestAssetCategory
{
	Unknown,
	Code, // 总之是需要编译的东西，cpp和h等
	// UE内部资产
	UEAsset,
	Texture2D,
	Material,
	StaticMesh,
	SkeletonMesh,
	Blueprint // 能够在UE内编译的蓝图
};

struct FAssetTestClassification
{
	// for different types of asset
	
	ETestAssetCategory Category = ETestAssetCategory::Unknown;

	FString CodeFilePath;
	
	FAssetData AssetData;

	bool IsAsset() const
	{
		return Category == ETestAssetCategory::UEAsset;
	}

	bool IsCode() const
	{
		return Category == ETestAssetCategory::Code;
	}

	bool IsBlueprint() const
	{
		return Category == ETestAssetCategory::Blueprint;
	}
};

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

	static UTexture2D* GetTextureByPath(FString FilePath);
	static FAssetData GetAssetDataByPath(FString FilePath);
	static TArray<UTexture2D*> GetTexturesByPath(TArray<FString> FilePaths);

	
	static bool CheckTexture(const TArray<UTexture2D*>& Textures,
		TArray<FString>& OutErrors,
		TArray<FString>& OutInfos,
		int32& PassCount,
		int32& FailCount);
	static bool CheckMaterial(const TArray<UTexture2D*>& Textures,
		TArray<FString>& OutErrors,
		TArray<FString>& OutInfos,
		int32& PassCount,
		int32& FailCount);
	static bool CheckCode(const TArray<UTexture2D*>& Textures,
		TArray<FString>& OutErrors,
		TArray<FString>& OutInfos,
		int32& PassCount,
		int32& FailCount);
	// for automation test
	static FAssetTestClassification ClassfyPath(const FString FilePaths);
	
	static ETestAssetCategory IdentifyAssetType(UObject* Asset);
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

