// Fill out your copyright notice in the Description page of Project Settings.

#include "GIANTS.h"

#include "CONTROLLER_BASE.h"



void ACONTROLLER_BASE::REFRESH(bool PUBLIC)
{
	GAMES_FOUND.Empty();

	Http = &FHttpModule::Get();
	if (!Http) return;
	if (!Http->IsHttpEnabled()) return;

	TSharedRef <IHttpRequest> Request = Http->CreateRequest();
	Request->SetVerb("POST");
	
	if (WITH_EDITOR)
	{
		Request->SetURL("http://pewpewcraft.com/include/_apps/_gameserver/getgames.aspx?debug=true&v=" + GET_BUILD_NUMBER_MAJOR_ONLY());
	}
	else
	{
		if (PUBLIC)
		{
			Request->SetURL("http://pewpewcraft.com/include/_apps/_gameserver/getgames.aspx?op=42ad5d32-9fec-4959-ad2e-203d422134a7&v=" + GET_BUILD_NUMBER_MAJOR_ONLY());
		}
		else
		{
			Request->SetURL("http://pewpewcraft.com/include/_apps/_gameserver/getgames.aspx?op=d7261114-cfc8-4e4a-8218-f3f83b3ece94&v=" + GET_BUILD_NUMBER_MAJOR_ONLY());
		}
	}

	Request->SetContentAsString("");
	Request->SetHeader("User-Agent", "ppcGameClient/0.0.6");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->OnProcessRequestComplete().BindUObject(this, &ACONTROLLER_BASE::REFRESH_RESPONSE);

	if (!Request->ProcessRequest())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "ProcessRequest Failed - Get Games");
	}
}

void ACONTROLLER_BASE::REFRESH_RESPONSE(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{


	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		LastError = "RESPONSE WAS NOT VALID";
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, LastError);
		REFRESH_ERROR();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FString MessageBody = "";
		MessageBody = Response->GetContentAsString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, MessageBody);
		FString left;
		FString right;

		while (MessageBody.Len() > 0)
		{
			if (MessageBody.Split("~", &left, &MessageBody))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, MessageBody);
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, left);
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, right);
				TArray<FString> tokens;
				left.ParseIntoArray(tokens, TEXT("|"), 1);

				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(tokens.Num()));

				FInternetGame game;
				game.IP = tokens[0];
				game.VERSION = tokens[1];
				game.PLAYERS_CURRENT = FCString::Atoi(*tokens[2]);
				game.PLAYERS_MAX = FCString::Atoi(*tokens[3]);
				game.GAME_NAME = tokens[4];
				GAMES_FOUND.Add(game);
			}
		}

		REFRESH_SUCCESS();
	}
	else
	{
		LastError = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, LastError);
		REFRESH_ERROR();
	}


}

void ACONTROLLER_BASE::JOIN_GAME(FString URL)
{
	this->ClientTravel(URL, ETravelType::TRAVEL_Absolute, false, FGuid());
}

bool ACONTROLLER_BASE::IS_PIE()
{
	if (WITH_EDITOR) return true;
	return false;
}

FString ACONTROLLER_BASE::GET_BUILD_NUMBER_MAJOR_ONLY()
{
	return "v0.1";
}