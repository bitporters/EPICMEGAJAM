// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Http.h"
#include "GAMEMODE_BASE.generated.h"

/**
 * 
 */
UCLASS()
class GIANTS_API AGAMEMODE_BASE : public AGameMode
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = SYSTEM_OVERRIDES)
	virtual void SERVER_REPORT(int32 CURRENT_PLAYERS);
};
