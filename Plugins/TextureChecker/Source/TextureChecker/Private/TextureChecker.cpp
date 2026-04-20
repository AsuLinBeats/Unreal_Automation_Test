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

TArray<FString> FTextureCheckerHelper::ReadChangelist(FString FileName)
{
	TArray<FString> FilePaths;
	FString FilePath = FPaths::ProjectDir() / FileName;
	
	if (!FFileHelper::LoadFileToStringArray(FilePaths, *FilePath))
	{
		UE_LOG(LogTemp,Warning,TEXT("File %s could not be read"),*FileName);
	}
	return FilePaths;
}

TArray<FString> FTextureCheckerHelper::ConvertGitPathsToUE(TArray<FString> Paths){
	TArray<FString> FilePaths;
	for (FString FilePath : Paths)
	{
		FilePaths.Add(FTextureCheckerHelper::ConvertGitPathToUE(FilePath));
	}
	return FilePaths;
}

FString FTextureCheckerHelper::ConvertGitPathToUE(FString Path){
	// Input path like: Content/xxxx, Source/xxxx. which is a Git root path，ie 本UE项目的相对根目录
	// 进行必要的清洗
	Path.ReplaceInline(TEXT("\\"), TEXT("/"));
	Path = Path.TrimStartAndEnd();
	// 判断File类型
	if (!Path.StartsWith("Content"))
	{
		return TEXT(""); // Non-Ue Content的情况先返回
	}
	if (!Path.EndsWith(".uasset"))
	{
		return TEXT("");
	}
	// 处理与拼接
	Path.RemoveFromStart(TEXT("Content/"));
	Path.RemoveFromEnd(TEXT(".uasset"));
	
	return FString::Printf(TEXT("/Game/%s"), *Path);
} 

/*
TArray<UTexture2D*> FTextureCheckerHelper::GetTexturesByPath(FString FilePath)
{
	TArray<UTexture2D*> Result;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*FilePath),AssetDataList);

	for (FAssetData& AssetData : AssetDataList){
		if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
		{
			Result.Add(Texture);
		}
	}
	return Result;
}
*/

// 可以改成数组操作版本
UTexture2D* FTextureCheckerHelper::GetTextureByPath(FString FilePath)
{
	TArray<UTexture2D*> Result;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	
	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FName(*FilePath));


	if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
	{
		return Texture;
		
	}
	return nullptr;

}

TArray<UTexture2D*> FTextureCheckerHelper::GetTexturesByPath(TArray<FString> FilePaths)
{
	TArray<UTexture2D*> Result;
	for (FString FilePath : FilePaths)
	{
		if (UTexture2D* Texture = FTextureCheckerHelper::GetTextureByPath(FilePath))
		{
			Result.Add(Texture);
		}
	}
	return Result;
}


bool FTextureCheckerHelper::CheckTexture(const TArray<UTexture2D*>& Textures, TArray<FString>& OutErrors,
                                         TArray<FString>& OutInfos, int32& PassCount, int32& FailCount)
{
	// reset
	PassCount = 0;
	FailCount = 0;

	for (UTexture2D* Texture : Textures)
	{
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
			OutInfos.Add(FString::Printf(TEXT("Qualified texture found: %s"),*Texture->GetName()));
		}else
		{
			FailCount++;
			OutErrors.Add(FString::Printf(TEXT("Unqualified texture found: %s"),*Texture->GetName()));
		}
	}

	return FailCount == 0;
}


bool FTextureTest::RunTest(const FString& Parameters)
{
	// 合并了下之前的情况
	// FString TargetPath = TEXT("/Game/Texture");
	// FName TargetName = FName(*TargetPath);
	// TArray<FString> TestOutput;
	
	// 读取changelist文件并转换格式
	TArray<FString> TargetPaths = FTextureCheckerHelper::ReadChangelist("ChangeList.txt");
	TargetPaths = FTextureCheckerHelper::ConvertGitPathsToUE(TargetPaths);
	for (FString TargetPath: TargetPaths)
	{
		FName TargetName = FName(*TargetPath);
		GetTexturesByPath(TargetName);
		
	}
	// 分辨类型
	
	

	
	TArray<FString> Infos;
	TArray<FString> Errors;
	int32 PassCount = 0;
	int32 ErrorCount = 0;

	// FOR NON-CODE ASSET
	TArray<UTexture2D*> Textures = FTextureCheckerHelper::GetTexturesByPath(TargetPaths);
	FTextureCheckerHelper::CheckTexture(Textures, Errors, Infos, PassCount, ErrorCount);
	

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
/*
bool FTextureTest::RunTest(const FString& Parameters)
{
	// 合并了下之前的情况
	// FString TargetPath = TEXT("/Game/Texture");
	// FName TargetName = FName(*TargetPath);
	// TArray<FString> TestOutput;
	
	// 读取changelist文件并转换格式
	TArray<FString> TargetPaths = FTextureCheckerHelper::ReadChangelist("ChangeList.txt");
	TargetPaths = FTextureCheckerHelper::ConvertGitPathsToUE(TargetPaths);
	FString TargetPath;
	FName TargetName = FName(*TargetPath);
	// 分辨类型
	
	TArray<FString> TestOutput;

	// FOR NON-CODE ASSET
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
*/

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextureCheckerModule, TextureChecker)