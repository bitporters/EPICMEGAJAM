// Fill out your copyright notice in the Description page of Project Settings.

#include "GIANTS.h"
#include "GAMEMODE_BASE.h"

void AGAMEMODE_BASE::SERVER_REPORT(int32 CURRENT_PLAYERS)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) return;
	if (!Http->IsHttpEnabled()) return;

	FString ProjectVersion = FString("v1.0");
	FString GameName = FString("DEDICATED 001");
	FString GameLocation = FString("USA-CENTRAL");
	bool IS_PUBLIC = false;
	int32 PLAYER_MAX = 0;

	if (GConfig)
	{
		GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
		GConfig->GetString(TEXT("/Script/ZTASettings.DedicatedServer"), TEXT("GameName"), GameName, GGameIni);
		GConfig->GetString(TEXT("/Script/ZTASettings.DedicatedServer"), TEXT("GameLocation"), GameLocation, GGameIni);
		GConfig->GetBool(TEXT("/Script/ZTASettings.DedicatedServer"), TEXT("IsPublic"), IS_PUBLIC, GGameIni);
		GConfig->GetInt(TEXT("/Script/ZTASettings.DedicatedServer"), TEXT("PlayerMax"), PLAYER_MAX, GGameIni);
	}


	//if (WITH_EDITOR)
	{
		//do nothing in the editor (for now)

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, ProjectVersion);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, GameName);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt((int)IS_PUBLIC));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::FromInt(PLAYER_MAX));
	}
	//else
	{
		FString qstring("?op=");
		qstring.Append("42ad5d32-9fec-4959-ad2e-203d422134a7&v=" + ProjectVersion + "&p=" + FString::FromInt(CURRENT_PLAYERS) + "&pmax=" + FString::FromInt(PLAYER_MAX) + "&n=" + GameName + "&n2=" + GameLocation);
		 
		TSharedRef <IHttpRequest> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		Request->SetURL("http://pewpewcraft.com/include/_apps/_gameserver/notify.aspx" + qstring);
		Request->SetContentAsString("");
		Request->SetHeader("User-Agent", "ZTAGAMECLIENT/1.20");
		Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");

		if (false) UE_LOG(LogClass, Warning, TEXT("URL REPORT - %s"), *qstring);

		/*
		Request->OnProcessRequestComplete().BindUObject(this, &Appc_mainMenuController::RefreshPPCServers_Reponse);
		*/
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogClass, Warning, TEXT("URL REPORT - %s"), "ProcessRequest Failed");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "ProcessRequest Failed - REPORT GAME");
		}
	}
}

