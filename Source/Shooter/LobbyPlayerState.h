// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

private:


public:

	virtual void SetPlayerName(const FString& S) override;
	

};
