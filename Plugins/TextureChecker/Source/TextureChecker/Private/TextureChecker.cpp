// Copyright Epic Games, Inc. All Rights Reserved.

#include "TextureChecker.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "FTextureCheckerModule"

void FTextureCheckerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTextureCheckerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


bool FSimpleTest::RunTest(const FString& Parameters)
{
	int32 Result = 2+1;
	TestEqual(TEXT("2+2 should equal 4"), Result, 4);

	return true;
}

// bool FTextureTest::RunTest(const FString& Parameters)
// {
// 	FString TargetPath = TEXT("/Game/Texture");
// 	FName TargetName = FName(*TargetPath);
// 	TArray<FString> TestOutput;
//
// 	// 资源获取
// 	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
// 	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
//
// 	TArray<FAssetData> AssetDataList;
// 	AssetRegistryModule.Get().GetAssetsByPath(TargetName,AssetDataList);
//
// 	int32 ErrorCount = 0;
// 	for (FAssetData& AssetData : AssetDataList)
// 	{
// 		// UTexture2d是真正的贴图，UTexture只是个基类，所以这里直接这么写
// 		UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()); // 2D
// 		if (!Texture)
// 		{
// 			continue;
// 		} 
// 		// 判断尺寸
// 		int32 Width = Texture->GetSizeX();
// 		int32 Height = Texture->GetSizeY();
// 		if (Width <= 0 && Height <= 0) continue;
// 		
// 		if (Width != Height)
// 		{
// 			ErrorCount++;
// 			AddError(FString::Printf(TEXT("Unqualified texture found: %s"),*Texture->GetName()));
// 		}
// 	}
// 	if (ErrorCount > 0)
// 	{
// 		AddError((FString::Printf(TEXT("Number of unqualified texture: %d"), ErrorCount)));
// 	}
// 	return true;
// }

bool FTextureTest::RunTest(const FString& Parameters)
{
	// 合并了下之前的情况
	FString TargetPath = TEXT("/Game/Texture");
	FName TargetName = FName(*TargetPath);
	TArray<FString> TestOutput;

	// 资源获取
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(TargetName,AssetDataList);

	int32 PassCount = 0;
	int32 ErrorCount = 0;
	for (FAssetData& AssetData : AssetDataList)
	{
		// UTexture2d是真正的贴图，UTexture只是个基类，所以这里直接这么写
		UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()); // 2D
		if (!Texture)
		{
			continue;
		} 
		// 判断尺寸
		int32 Width = Texture->GetSizeX();
		int32 Height = Texture->GetSizeY();
		if (Width <= 0 && Height <= 0) continue;
		
		if (Width != Height)
		{
			PassCount++;
			AddInfo(FString::Printf(TEXT("Qualified texture found: %s"),*Texture->GetName()));
		}else
		{
			ErrorCount++;
			AddError(FString::Printf(TEXT("Unqualified texture found: %s"),*Texture->GetName()));
		}
	}
	if (PassCount > 0)
	{
		AddInfo((FString::Printf(TEXT("Number of qualified texture: %d"), PassCount)));
	}
	if (ErrorCount > 0)
	{
		AddError((FString::Printf(TEXT("Number of unqualified texture: %d"), ErrorCount)));\
		return false;
	}
	return true;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextureCheckerModule, TextureChecker)