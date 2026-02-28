// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Misc/AutomationTest.h"



class FTextureCheckerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
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

