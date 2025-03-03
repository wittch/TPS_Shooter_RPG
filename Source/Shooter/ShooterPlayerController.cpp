#include "ShooterPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"
#include "ShooterPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "ShooterCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
AShooterPlayerController::AShooterPlayerController()
{
    TSubclassOf<UUserWidget> NewWidget;
    NewWidget = ServerWidget;

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidget != nullptr)
    {
        CurrentWidget = CreateWidget(GetWorld(), NewWidget);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }

}
void AShooterPlayerController::ShooterDefeatWidget()
{
    TSubclassOf<UUserWidget> NewWidget;
    NewWidget = DefeatWidget;

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidget != nullptr)
    {
        CurrentWidget = CreateWidget(GetWorld(), NewWidget);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
    this->SetInputMode(FInputModeUIOnly());
    this->bShowMouseCursor = true;

}
void AShooterPlayerController::ShooterServerWidget()
{

    TSubclassOf<UUserWidget> NewWidget;
    NewWidget = ServerWidget;

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidget != nullptr)
    {
        CurrentWidget = CreateWidget(GetWorld(), NewWidget);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
    this->SetInputMode(FInputModeUIOnly());
    this->bShowMouseCursor = true;
}
void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeGameOnly());
    ShooterMainWidget(ShooterWidget);
}

void AShooterPlayerController::ShooterMainWidget(TSubclassOf<UUserWidget> NewWidget)
{

    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidget != nullptr)
    {
        CurrentWidget = CreateWidget(GetWorld(), NewWidget);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }

}
