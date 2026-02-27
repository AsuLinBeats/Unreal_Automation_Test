// Copyright Epic Games, Inc. All Rights Reserved.

#include "TextureChecker.h"

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


bool FTextureTest::RunTest(const FString& Parameters)
{
	int32 Result = 2+2;
	TestEqual(TEXT("2+2 should equal 4"), Result, 4);

	return true;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextureCheckerModule, TextureChecker)