// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"
#include "ShooterPlayerState.h"
#include "LobbyPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"


ALobbyPlayerController::ALobbyPlayerController()
{
}
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeUIOnly());
	this->bShowMouseCursor = true;

	LobbyWidget(WidgetTemplate);

}
void ALobbyPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

}



void ALobbyPlayerController::LobbyWidget(TSubclassOf<UUserWidget> NewWidget)
{
	if (WidgetInstance != nullptr)
	{
		WidgetInstance->RemoveFromViewport();
		WidgetInstance = nullptr;
	}

	if (NewWidget != nullptr)
	{
		WidgetInstance = CreateWidget(GetWorld(), NewWidget);
		if (WidgetInstance != nullptr)
		{
			WidgetInstance->AddToViewport();
		}
	}
}
