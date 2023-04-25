// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"
#include "Containers/Array.h"
#include "LobbyPlayerState.h"

UShooterGameInstance::UShooterGameInstance()
{
	
}

void UShooterGameInstance::SetPlayerName(FString Name)
{
	LobbyPlayerState->SetPlayerName(Name);
}

FString UShooterGameInstance::GetPlayerName()
{
	return LobbyPlayerState->GetPlayerName();
}
