// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Ammo.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Shooter.h"
#include "BulletHitInterface.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "ShooterAnimInstance.h"
#include "ShooterPlayerController.h"


// Sets default values
AShooterCharacter::AShooterCharacter() :
	// Base rates for turning/looking up
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	// Turn rates for aiming/not aiming
	HipTurnRate(45.f),
	HipLookUpRate(45.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	// Mouse look sensitivity scale factors
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f),
	// true when aiming the weapon
	bAiming(false),
	// Camera field of view values
	CameraDefaultFOV(0.f), // set in BeginPlay
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(20.f),
	// Crosshair variables
	CrosshairSpreadMax(16.f),
	CrosshairW(64.f),
	CrosshairH(64.f),
	// Crosshair spread factors
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelocityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	// Bullet fire timer variables
	ShootTimeDuration(0.15f),
	bFiringBullet(false),
	// Automatic fire variables
	bFireButtonPressed(false),
	bShouldFire(true),
	// Item trace variables
	bShouldTraceForItems(false),
	OverlappedItemCount(0),
	// Starting ammo amount
	Starting9mmAmmo(85),
	StartingARAmmo(120),
	// Combat variables
	CombatState(ECombatState::ECS_Unoccupied),
	// Crouching variables
	bCrouching(false),
	BaseMovementSpeed(650.f),
	CrouchMovementSpeed(300.f),
	CurrentCapsuleHalfHeight(88.f),
	StandingCapsuleHalfHeight(88.f),
	CrouchingCapsuleHalfHeight(44.f),
	BaseGroundFriction(2.f),
	CrouchingGroundFriction(100.f),
	BaseBrakingDecelerationWalking(65.f),
	CrouchingBrakingDecelerationWalking(2048.f),
	// Aiming
	bAimingButtonPressed(false),
	// Pickup sound timer properties
	bShouldPlayPickupSound(true),
	bShouldPlayEquipSound(true),
	PickupSoundResetTime(0.2f),
	EquipSoundResetTime(0.2f),
	// Icon animation property
	HighlightedSlot(-1),
	// Combat variables
	Health(100.f),
	MaxHealth(100.f),
	StunChance(.25f),
	//replicate
	bReplicateShooterMovement(true),
	AnimInstance(nullptr),
	LookUpScaleFactor(0.0f)

{


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the character if there is a collision) 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // 실제 탬플릿 타입에 해당하는 서브 오브젝트를 설정한 이름으로 생성하여 반환
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Don't rotate when the controller rotates. Let the controller only affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create Hand Scene Component
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComp"));

	// Create Interpolation Component
	WeaponInterpComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Interpolation Component"));
	WeaponInterpComp->SetupAttachment(GetFollowCamera());

	InterpComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 1"));
	InterpComp1->SetupAttachment(GetFollowCamera());

	InterpComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 2"));
	InterpComp2->SetupAttachment(GetFollowCamera());

	InterpComp3 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 3"));
	InterpComp3->SetupAttachment(GetFollowCamera());

	InterpComp4 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 4"));
	InterpComp4->SetupAttachment(GetFollowCamera());

	InterpComp5 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 5"));
	InterpComp5->SetupAttachment(GetFollowCamera());

	InterpComp6 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 6"));
	InterpComp6->SetupAttachment(GetFollowCamera());


	bReplicates = true;
	SetReplicateMovement(bReplicates);
	//SetReplicates(bReplicates);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die();

		auto EnemyController = Cast<AEnemyController>(EventInstigator);
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				FName(TEXT("CharacterDead")),
				true);
		}
		APlayerController* pc2 = Cast<APlayerController>(GetController());

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);


		for (auto iter = GetWorld()->GetPlayerControllerIterator(); iter; iter++)
		{
			AShooterPlayerController* spc = Cast<AShooterPlayerController>(*iter);
			
			if (PC == pc2) 
			{
				if (!GetWorld()->IsServer())
				{
					bool tmp = true;
					spc->ShooterDefeatWidget();
					for (auto i = GetWorld()->GetPlayerControllerIterator(); i; i++)
					{
						AShooterCharacter* SC = Cast<AShooterCharacter>(i->Get()->GetPawn());
						if (SC->GetCombatState() != ECombatState::ECS_Dying)
						{
							tmp = false;
						}

					}
					if (tmp)
					{
						AShooterPlayerController* tmppc = Cast<AShooterPlayerController>(PC);
						tmppc->ShooterServerWidget();
					}

				}
				else
				{
					bool tmp = true;
					
					for (auto i = GetWorld()->GetPlayerControllerIterator(); i; i++)
					{
						AShooterCharacter* SC = Cast<AShooterCharacter>(i->Get()->GetPawn());
						if (SC->GetCombatState() != ECombatState::ECS_Dying)
						{
							tmp = false;
						}

					}
					if (tmp)
					{
						AShooterPlayerController* tmppc = Cast<AShooterPlayerController>(PC);
						tmppc->ShooterServerWidget();
					}
					
				}
			}
			else if (GetWorld()->IsServer())
			{
				bool tmp = true;

				for (auto i = GetWorld()->GetPlayerControllerIterator(); i; i++)
				{
					AShooterCharacter* SC = Cast<AShooterCharacter>(i->Get()->GetPawn());
					if (SC->GetCombatState() != ECombatState::ECS_Dying)
					{
						tmp = false;
					}

				}
				if (tmp)
				{
					AShooterPlayerController* tmppc = Cast<AShooterPlayerController>(PC);
					tmppc->ShooterServerWidget();
				}
			}
		}


	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	// Spawn the default weapon and attach it to the mesh

	/*if (HasAuthority())
	{
		// 각 캐릭터의 디폴트 웨폰을 서버가 생성.
		EquippedWeapon = SpawnDefaultWeapon();
		// 각 캐릭터에 이큅드웨폰을 서버가 지정.
		// 각 캐릭터의 이큅드웨폰가 서버에 노티파이되어있어야함.
		// EquippedWeapon을 서버가 각 캐릭터에 이큅시킴
	}*/


	ResSpawnDefaultWeapon();
	Inventory.SetNum(INVENTORY_CAPACITY);
	Inventory[DefaultSlotIndex] = EquippedWeapon;
	EquippedWeapon->DisableCustomDepth();
	EquippedWeapon->DisableGlowMaterial();
	EquippedWeapon->SetCharacter(this);

	InitializeAmmoMap();
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;


	InitializeInterpLocations();


}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor{};
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	LookUpScaleFactor;
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::FireWeapon()
{
	ReqFireWeapon();
	if (WeaponHasAmmo())
		SendBullet();


}

void AShooterCharacter::ReqFireWeapon_Implementation()
{
	ResFireWeapon();
}

void AShooterCharacter::ResFireWeapon_Implementation()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied && CombatState != ECombatState::ECS_Reloading) return;
	if (CombatState == ECombatState::ECS_Reloading)
	{
		AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Pause(ReloadMontage);
			AnimInstance->Montage_JumpToSection(
				EquippedWeapon->GetReloadMontageSection());
		}
	}

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		StartFireTimer();
		PlayGunFireMontage();

		// Start bullet fire timer for crosshairs
		StartCrosshairBulletFire();
		// Subtract 1 from the Weapon's Ammo
		EquippedWeapon->DecrementAmmo();

		RecoilCameraShake();

		//StartFireTimer();
		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol)
		{
			// Start moving slide timer
			EquippedWeapon->StartSlideTimer();
		}
	}
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation,
	FHitResult& OutHitResult)
{
	FVector OutBeamLocation;

	// Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation, 50'000.f, true);

	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f };

	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);

	if (!OutHitResult.bBlockingHit) // object between barrel and BeamEndPoint?
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}

	return true;
}

void AShooterCharacter::AimingButtonPressed()
{
	ReqAimingButtonPressed();
}

void AShooterCharacter::ReqAimingButtonPressed_Implementation()
{
	bAimingButtonPressed = true;
	if (CombatState == ECombatState::ECS_Unoccupied)
	{
		Aim();
	}
}

void AShooterCharacter::AimingButtonReleased()
{
	ReqAimingButtonReleased();
}

void AShooterCharacter::ReqAimingButtonReleased_Implementation()
{
	bAimingButtonPressed = false;
	StopAiming();
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	// Set current camera field of view
	if (bAiming) // Aiming button pressed?
	{
		// Interpolate to zoomed FOV
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		// Interpolate to default FOV
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AShooterCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f,600.f };
	FVector2D VelocityMultiplierRange{ 0.f,1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	// Calculate crosshair velocity factor
	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());

	// Calculate crosshair in air factor
	if (GetCharacterMovement()->IsFalling()) // is in air?
	{
		// Spread the crosshairs slowly while in air
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else // Character is on the ground
	{
		// Shrink the crosshairs rapidly while on the ground
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	// Calculate crosshair aim factor
	if (bAiming)
	{
		// Shrink crosshairs a small amount very quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.5f,
			DeltaTime,
			30.f);
	}
	else
	{
		// Spread crosshairs back to normal very quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			EquippedWeapon->GetBaseFireSpreadFactor(),
			DeltaTime,
			30.f);
	}

	// True 0.05f second after firing
	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.3f,
			DeltaTime,
			30.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.f,
			DeltaTime,
			30.f);

	}

	CrosshairSpreadMultiplier =
		EquippedWeapon->GetBaseSpreadFactor() +
		CrosshairVelocityFactor +
		CrosshairInAirFactor -
		CrosshairAimFactor +
		CrosshairShootingFactor;
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	if (WeaponHasAmmo())
	{
		FireWeapon();
	}
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartFireTimer()
{
	if (EquippedWeapon == nullptr) return;

	CombatState = ECombatState::ECS_FireTimerInProgress;
	GetWorldTimerManager().SetTimer(
		AutoFireTimer,
		this,
		&AShooterCharacter::AutoFireReset,
		EquippedWeapon->AutoFireRate);
}

void AShooterCharacter::AutoFireReset()
{
	if (CombatState != ECombatState::ECS_FireTimerInProgress) return;

	CombatState = ECombatState::ECS_Unoccupied;
	if (EquippedWeapon == nullptr) return;
	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed && EquippedWeapon->bAutomatic)
		{
			FireWeapon();
		}
	}
	else
	{
		// Reload Weapon
		if (bFireButtonPressed && EquippedWeapon->bAutomatic)
		{
			ReloadWeapon();
		}
	}
}

void AShooterCharacter::StartCrosshairBulletFire()
{
	SetFiringBullet(true);

	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer,
		this,
		&AShooterCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}

void AShooterCharacter::FinishCrosshairBulletFire()
{
	SetFiringBullet(false);
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation, float TraceLength, bool bShooting)
{
	// Get Viewport Size
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	if (bShooting)
	{
		// SpreadMultiplier == 0 -> Default
		float OffsetHalf = CrosshairSpreadMultiplier * CrosshairSpreadMax;
		float OffsetX = FMath::RandRange(-OffsetHalf, OffsetHalf);
		float OffsetY = FMath::RandRange(-OffsetHalf, OffsetHalf);
		CrosshairLocation += FVector2D(OffsetX, OffsetY);
	}

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get World position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);
	CrosshairWorldDirection.Normalize();

	if (bScreenToWorld)
	{
		// Trace from Crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * TraceLength };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 20.f);
		DrawDebugPoint(GetWorld(), OutHitResult.Location, 10.f, FColor::Red, false, 20.f);*/

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void AShooterCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector ItemTraceLocation;
		TraceUnderCrosshairs(ItemTraceResult, ItemTraceLocation, 400.f, false);
		if (ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);
			const auto TraceHitWeapon = Cast<AWeapon>(TraceHitItem);
			bool bHasEmptySlot;
			GetChangableInventorySlot(bHasEmptySlot);
			if (TraceHitWeapon && bHasEmptySlot)
			{
				if (HighlightedSlot == -1)
				{
					// Not currently highlighting a slot; highlight one
					HighlightInventorySlot();
				}
			}
			else
			{
				// Is a slot being highlight?
				if (HighlightedSlot != -1)
				{
					// UnHighlight the slot
					UnHighlightInventorySlot();
				}
			}

			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}
			if (TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				// Show Item's Pickup Widet
				TraceHitItem->GetPickupWidget()->SetVisibility(true);
				TraceHitItem->EnableCustomDepth();


			}

			// We hit an AItem last frame
			if (TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					// We are hitting a different AItem this frame from last frame
					// Or AItem is null
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->DisableCustomDepth();
				}
			}

			// Store a reference to HitItem for next frame
			TraceHitItemLastFrame = TraceHitItem;
		}
		else
		{

			if (TraceHitItem && TraceHitItemLastFrame)
			{
				TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
				TraceHitItemLastFrame->DisableCustomDepth();
				TraceHitItemLastFrame = nullptr;
			}
			TraceHitItem = nullptr;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		// No longer overlapping any items,
		// Item last frame should not show widget
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
		TraceHitItemLastFrame->DisableCustomDepth();
		TraceHitItemLastFrame = nullptr;
		TraceHitItem = nullptr;
	}
}

AWeapon* AShooterCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		// Spawn the Weapon
		AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
		if (DefaultWeapon->GetBoneToHide() != FName(""))
		{
			DefaultWeapon->GetItemMesh()->UnHideBoneByName(DefaultWeapon->GetBoneToHide());
		}
		DefaultWeapon->SlotIndex = (DefaultSlotIndex);
		DefaultWeapon->SetItemRarity(EItemRarity::EIR_Default);
		DefaultWeapon->SetWeaponType(EWeaponType::EWT_Pistol);
		if (DefaultWeapon->GetBoneToHide() != FName(""))
		{
			DefaultWeapon->GetItemMesh()->HideBoneByName(DefaultWeapon->GetBoneToHide(), EPhysBodyOp::PBO_None);
		}
		return DefaultWeapon;
	}

	return nullptr;
}

void AShooterCharacter::ResSpawnDefaultWeapon_Implementation()
{
	EquipWeapon(SpawnDefaultWeapon());
}

void AShooterCharacter::EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping)
{
	if (WeaponToEquip)
	{
		// Get the Hand Socket
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(
			FName("RightHandSocket"));
		if (HandSocket)
		{
			// Attach the Weapon to the hand socket RightHandSocket
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}

		if (EquippedWeapon == nullptr)
		{
			// -1 == no EquippedWeapon yet. No need to reverse the icon animation
			EquipItemDelegate.Broadcast(-1, WeaponToEquip->SlotIndex);
		}
		else if (!bSwapping)
		{
			EquipItemDelegate.Broadcast(EquippedWeapon->SlotIndex, WeaponToEquip->SlotIndex);
		}

		// Set EquippedWeapon to the new Weapon
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);

		CombatState = ECombatState::ECS_Equipping;
		AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EquipMontage)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Equip"));
		}
		EquippedWeapon->PlayEquipSound(true);

		ShootTimeDuration = EquippedWeapon->GetShootTimeDuration();
	}
}

void AShooterCharacter::DropWeapon(int32 SelectedIndex, bool bSwapping)
{
	if (!EquippedWeapon)return;

	AWeapon* SelectedWeapon = Cast<AWeapon>(Inventory[SelectedIndex]);
	if (!SelectedWeapon) return;

	FDetachmentTransformRules DetachmentTransfromRules(EDetachmentRule::KeepWorld, true);
	SelectedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransfromRules);
	SelectedWeapon->SetItemState(EItemState::EIS_Falling);
	SelectedWeapon->ThrowWeapon();
	if (bSwapping) // is swapping now => when E Key pressed and no more slot
	{
		// Drop EquippedWeapon Or Drop Inventory Weapon
	}
	else // is not swapping now => when Q key pressed and no need to swap
	{
		// Eqip Next Weapon
		int32 WeaponIndex = -1;
		for (WeaponIndex = 0; WeaponIndex < INVENTORY_CAPACITY; ++WeaponIndex)
		{
			if (WeaponIndex == SelectedIndex)continue;
			if (Inventory[WeaponIndex] != nullptr) break;
		}
		if (WeaponIndex >= 0 && WeaponIndex < INVENTORY_CAPACITY)
		{
			// Equip next weapon
			AWeapon* NewWeapon = Cast<AWeapon>(Inventory[WeaponIndex]);
			if (NewWeapon)
			{
				EquipWeapon(NewWeapon);
			}
		}
		Inventory[SelectedIndex] = nullptr;
	}

	SelectedWeapon->SetOwningCharacter(nullptr);
	SelectedWeapon->SlotIndex = (-1);
}

void AShooterCharacter::SelectButtonPressed()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (TraceHitItem)
	{
		TraceHitItem->StartItemCurve(this, true);
		TraceHitItem = nullptr;
	}
}

void AShooterCharacter::SelectButtonReleased()
{
}

void AShooterCharacter::SwapWeapon(AWeapon* WeaponToSwap, int32 SelectedIndex)
{
	bool bSwapWithEquippedWeapon = (SelectedIndex == EquippedWeapon->SlotIndex);

	// TODO:: Pickedup or Equip weapon
	if (bSwapWithEquippedWeapon) // Swap with Equipped Weapon
	{
		DropWeapon(SelectedIndex, true);
		EquipWeapon(WeaponToSwap, true);
	}
	else // Swap with Inventory Weapon
	{
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(
			FName("RightHandSocket"));
		const FTransform SocketTransform = HandSocket->GetSocketTransform(GetMesh());
		Inventory[SelectedIndex]->GetItemMesh()->SetWorldTransform(SocketTransform);
		DropWeapon(SelectedIndex, true);
		WeaponToSwap->SetItemState(EItemState::EIS_Pickedup);
	}
	Inventory[SelectedIndex] = WeaponToSwap;
	WeaponToSwap->SlotIndex = (SelectedIndex);


	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void AShooterCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);
}

bool AShooterCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	return EquippedWeapon->GetAmmo() > 0;
}

void AShooterCharacter::PlayFireSound()
{
	// Play fire sound
	if (EquippedWeapon->GetFireSound())
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetFireSound(), GetActorLocation());
	}
}

void AShooterCharacter::SendBullet()
{
	const USkeletalMeshSocket* BarrelSocket =
		EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

		FHitResult BeamHitResult;
		bool bBeamEnd = GetBeamEndLocation(
			SocketTransform.GetLocation(), BeamHitResult);
		if (bBeamEnd)
		{
			// Does hit Actor implement BulletHitInterface?
			if (BeamHitResult.Actor.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("GunShot Hit %s Actor!"), *BeamHitResult.Actor.Get()->GetName());

				IBulletHitInterface* BulletHitinterface = Cast<IBulletHitInterface>(BeamHitResult.Actor.Get());
				if (BulletHitinterface)
				{
					ReqBulletHit(BeamHitResult);
				}
				else
				{
					// Spawn default particles
					if (EquippedWeapon->GetImpactParticles())
					{
						UGameplayStatics::SpawnEmitterAtLocation(
							GetWorld(),
							EquippedWeapon->GetImpactParticles(),
							BeamHitResult.Location);
					}
				}
			}
		}
		ReqSendBullet(SocketTransform, BeamHitResult);
	}


}

void AShooterCharacter::ReqSendBullet_Implementation(const FTransform SocketTransform, const FHitResult BeamHitResult)
{
	ResSendBullet(SocketTransform, BeamHitResult);
}

void AShooterCharacter::ResSendBullet_Implementation(const FTransform SocketTransform, const FHitResult BeamHitResult)
{


	if (EquippedWeapon->GetMuzzleFlash())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);
	}

	AEnemy* HitEnemy = Cast<AEnemy>(BeamHitResult.Actor.Get());
	if (HitEnemy)
	{
		int32 Damage{};
		bool bHeadShot;
		UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *BeamHitResult.BoneName.ToString());
		if (BeamHitResult.BoneName.ToString() == HitEnemy->GetHeadBone())
		{
			// Head shot
			Damage = EquippedWeapon->GetHeadShotDamage();
			bHeadShot = true;
		}
		else
		{
			// Body shot
			Damage = EquippedWeapon->GetDamage();
			bHeadShot = false;
		}
		UGameplayStatics::ApplyDamage(
			BeamHitResult.Actor.Get(),
			Damage,
			GetController(),
			this,
			UDamageType::StaticClass());
		HitEnemy->ShowHitNumber(Damage, BeamHitResult.Location, bHeadShot);
	}

	if (EquippedWeapon->GetBeamParticles())
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			EquippedWeapon->GetBeamParticles(),
			SocketTransform);
		if (Beam)
		{
			Beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
		}
	}
}

void AShooterCharacter::PlayGunFireMontage()
{
	// Play Hip Fire Montage
	AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

void AShooterCharacter::ReloadButtonPressed()
{
	//if (UGameplayStatics::GetPlayerController(this, 0) == GetController())
	//{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s->ReloadButtonPressed()"), *GetName()));
	ReloadWeapon();
	//}
}

void AShooterCharacter::ReloadWeapon()
{
	ReqReloadWeapon();

}

void AShooterCharacter::ReqReloadWeapon_Implementation()
{
	ResReloadWeapon();

}

void AShooterCharacter::ResReloadWeapon_Implementation()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;


	if (EquippedWeapon == nullptr) return;

	// Do we have ammo of the correct type?
	if (CarryingAmmo() && !EquippedWeapon->ClipIsFull())
	{
		if (bAiming)
		{
			StopAiming();
		}
		CombatState = ECombatState::ECS_Reloading;
		AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(
				EquippedWeapon->GetReloadMontageSection());
		}


	}
}
bool AShooterCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	if (EquippedWeapon->GetItemRarity() == EItemRarity::EIR_Default) return true;
	auto AmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void AShooterCharacter::GrabClip()
{
	//if (UGameplayStatics::GetPlayerController(this, 0) != GetController()) return;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s->GrabClip()"), *GetName()));


	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;

	// Index for the clip bone on the Equipped Weapon
	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName()) };
	// Store the transform of the clip
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Hand_L")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AShooterCharacter::ReleaseClip()
{
	//if (UGameplayStatics::GetPlayerController(this, 0) != GetController()) return;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%s->ReleaseClip()"), *GetName()));
	EquippedWeapon->SetMovingClip(false);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//UAnimInstance* AnimInstance;  == Montage Replicate
	DOREPLIFETIME(AShooterCharacter, AnimInstance);
	DOREPLIFETIME(AShooterCharacter, bReplicateShooterMovement);
	DOREPLIFETIME(AShooterCharacter, bAiming);

	DOREPLIFETIME(AShooterCharacter, CombatState);
	DOREPLIFETIME(AShooterCharacter, bCrouching);
	DOREPLIFETIME(AShooterCharacter, bFiringBullet);

	DOREPLIFETIME(AShooterCharacter, DefaultWeaponClass);
	//DOREPLIFETIME(AShooterCharacter, EquippedWeapon);
	DOREPLIFETIME(AShooterCharacter, CurrentCapsuleHalfHeight);
	DOREPLIFETIME(AShooterCharacter, ReloadMontage);
	DOREPLIFETIME(AShooterCharacter, AutoFireTimer);




}

void AShooterCharacter::SetReplicateMovement(bool bInReplicateMovement)
{
	Super::SetReplicateMovement(bInReplicateMovement);
}

void AShooterCharacter::UpdateHP(float CurHp, float MaxHP)
{
	Health = CurHp;
	MaxHealth = MaxHP;
}

void AShooterCharacter::UpdateCombatState(ECombatState state)
{
	CombatState = state;
}

void AShooterCharacter::BindPlayerState()
{
	UpdateHpDelegate.AddUFunction(this, FName("UpdateHP"));
	UpdateCombatStateDelegate.BindUFunction(this, FName("UpdateCombatState"));
}

void AShooterCharacter::ReqBulletHit_Implementation(FHitResult BeamHit)
{
	ResBulletHit(BeamHit);
}

void AShooterCharacter::ResBulletHit_Implementation(FHitResult BeamHit)
{
	IBulletHitInterface* BulletHitinterface = Cast<IBulletHitInterface>(BeamHit.Actor.Get());
	BulletHitinterface->BulletHit_Implementation(BeamHit, this, GetController());
}

void AShooterCharacter::SetFiringBullet_Implementation(bool state)
{
	ResSetFiringBullet(state);
}

void AShooterCharacter::ResSetFiringBullet_Implementation(bool state)
{
	bFiringBullet = state;
}

void AShooterCharacter::SetCrouching(bool state)
{
	bCrouching = state;
}



void AShooterCharacter::ReqSetCrouchingPressed_Implementation()
{
	ResSetCrouchingPressed();
}

void AShooterCharacter::ResSetCrouchingPressed_Implementation()
{

	if (!GetCharacterMovement()->IsFalling())
	{
		SetCrouching(!bCrouching);
	}
	if (bCrouching)
	{

		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
		GetCharacterMovement()->BrakingDecelerationWalking = CrouchingBrakingDecelerationWalking;
		GetCharacterMovement()->Crouch(true);

	}
	else
	{


		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
		GetCharacterMovement()->BrakingDecelerationWalking = BaseBrakingDecelerationWalking;
		GetCharacterMovement()->UnCrouch(true);
	}
}

void AShooterCharacter::SetAiming_Implementation(bool state)
{
	ResSetAiming(state);
}


void AShooterCharacter::ResSetAiming_Implementation(bool state)
{
	bAiming = state;
}
void AShooterCharacter::RecoilCameraShake()
{
}
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle interpolation for zoom when aiming
	CameraInterpZoom(DeltaTime);
	// Change look sensitivity based on aiming
	SetLookRates();
	// Calculate crosshair spread multiplier
	CalculateCrosshairSpread(DeltaTime);
	// Check OverlappedItemCount, then trace for items
	TraceForItems();
	// Interpolate the capsule half height based on crouching/standing

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this,
		&AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this,
		&AShooterCharacter::FireButtonReleased);


	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this,
		&AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this,
		&AShooterCharacter::AimingButtonReleased);

	PlayerInputComponent->BindAction("Select", IE_Pressed, this,
		&AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this,
		&AShooterCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this,
		&AShooterCharacter::ReloadButtonPressed);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this,
		&AShooterCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("1Key", IE_Pressed, this,
		&AShooterCharacter::OneKeyPressed);
	PlayerInputComponent->BindAction("2Key", IE_Pressed, this,
		&AShooterCharacter::TwoKeyPressed);
	PlayerInputComponent->BindAction("3Key", IE_Pressed, this,
		&AShooterCharacter::ThreeKeyPressed);

	PlayerInputComponent->BindAction("Drop", IE_Pressed, this,
		&AShooterCharacter::DropKeyPressed);

}

void AShooterCharacter::FinishReloading()
{
	ReqFinishReloading();
}

void AShooterCharacter::ReqFinishReloading_Implementation()
{
	ResFinishReloading();
}

void AShooterCharacter::ResFinishReloading_Implementation()
{

	if (CombatState != ECombatState::ECS_Reloading) return;
	// Update the Combat State
	CombatState = ECombatState::ECS_Unoccupied;
	if (bAimingButtonPressed)
	{
		Aim();
	}

	if (EquippedWeapon == nullptr) return;

	const auto AmmoType{ EquippedWeapon->GetAmmoType() };

	// Update AmmoMap
	if (EquippedWeapon->GetItemRarity() == EItemRarity::EIR_Default)
	{
		// default weapon does not use any ammo
		const int32 MagEmptySpace =
			EquippedWeapon->GetMagazineCapacity()
			- EquippedWeapon->GetAmmo();
		EquippedWeapon->ReloadAmmo(MagEmptySpace);
	}
	else if (AmmoMap.Contains(AmmoType))
	{
		// Amount of ammo the Character is carrying of the EquippedWeapon type
		int32 CarriedAmmo = AmmoMap[AmmoType];

		// Space left in the magazine of EquippedWeapon
		const int32 MagEmptySpace =
			EquippedWeapon->GetMagazineCapacity()
			- EquippedWeapon->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			// Reload the magazine with all the ammo we are carrying
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			// fill the magazine
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}

}
void AShooterCharacter::FinishEquipping()
{
	if (CombatState != ECombatState::ECS_Equipping) return;
	ReqFinishEquipping();
}

void AShooterCharacter::ReqFinishEquipping_Implementation()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (bAimingButtonPressed)
	{
		Aim();
	}
}

void AShooterCharacter::CrouchButtonPressed()
{
	ReqSetCrouchingPressed();
}

void AShooterCharacter::Jump()
{
	if (bCrouching)
	{
		SetCrouching(false);
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
		GetCharacterMovement()->BrakingDecelerationWalking = BaseBrakingDecelerationWalking;

	}
	else
	{
		ACharacter::Jump();
	}
}




void AShooterCharacter::Aim()
{

	SetAiming(true);

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void AShooterCharacter::StopAiming()
{
	SetAiming(false);
	if (!bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::PickupAmmo(AAmmo* Ammo)
{
	// check to see if AmmoMap contains Ammo's AmmoType
	if (AmmoMap.Find(Ammo->GetAmmoType()))
	{
		// Get amount of ammo in our AmmoMap for Ammo's type
		int32 AmmoCount{ AmmoMap[Ammo->GetAmmoType()] };
		AmmoCount += Ammo->GetItemCount();
		// Set the amount of ammo in the Map for this type
		AmmoMap[Ammo->GetAmmoType()] = AmmoCount;
	}

	if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType())
	{
		// Check to see if the gun is empty
		if (EquippedWeapon->GetAmmo() == 0)
		{
			ReloadWeapon();
		}
	}

	Ammo->Destroy();
}

void AShooterCharacter::InitializeInterpLocations()
{
	FInterpLocation WeaponLocation{ WeaponInterpComp,0 };
	InterpLocations.Add(WeaponLocation);

	FInterpLocation InterpLoc1{ InterpComp1,0 };
	InterpLocations.Add(InterpLoc1);
	FInterpLocation InterpLoc2{ InterpComp2,0 };
	InterpLocations.Add(InterpLoc2);
	FInterpLocation InterpLoc3{ InterpComp3,0 };
	InterpLocations.Add(InterpLoc3);
	FInterpLocation InterpLoc4{ InterpComp4,0 };
	InterpLocations.Add(InterpLoc4);
	FInterpLocation InterpLoc5{ InterpComp5,0 };
	InterpLocations.Add(InterpLoc5);
	FInterpLocation InterpLoc6{ InterpComp6,0 };
	InterpLocations.Add(InterpLoc6);
}


void AShooterCharacter::OneKeyPressed()
{
	ExchangeInventoryItems(EquippedWeapon->SlotIndex, 0);
}

void AShooterCharacter::TwoKeyPressed()
{
	ExchangeInventoryItems(EquippedWeapon->SlotIndex, 1);
}

void AShooterCharacter::ThreeKeyPressed()
{
	ExchangeInventoryItems(EquippedWeapon->SlotIndex, 2);
}

void AShooterCharacter::DropKeyPressed()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (!EquippedWeapon || EquippedWeapon->SlotIndex == DefaultSlotIndex) return;
	DropWeapon(EquippedWeapon->SlotIndex, false);
}

void AShooterCharacter::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{
	ReqExchangeInventoryItems(CurrentItemIndex, NewItemIndex);
}

void AShooterCharacter::ReqExchangeInventoryItems_Implementation(int32 CurrentItemIndex, int32 NewItemIndex)
{
	ResExchangeInventoryItems(CurrentItemIndex, NewItemIndex);
}

void AShooterCharacter::ResExchangeInventoryItems_Implementation(int32 CurrentItemIndex, int32 NewItemIndex)
{
	const bool bCanExchangeItems =
		(CurrentItemIndex != NewItemIndex) &&
		(Inventory[NewItemIndex] != nullptr) &&
		(CombatState == ECombatState::ECS_Unoccupied || CombatState == ECombatState::ECS_Equipping);




	if (bCanExchangeItems)
	{
		if (bAiming)
		{
			StopAiming();
		}
		auto OldEquippedWeapon = EquippedWeapon;
		auto NewWeapon = Cast<AWeapon>(Inventory[NewItemIndex]);
		EquipWeapon(NewWeapon);

		OldEquippedWeapon->SetItemState(EItemState::EIS_Pickedup);

	}
}

int32 AShooterCharacter::GetChangableInventorySlot(bool& OutHasEmptySlot)
{

	int32 ChangableIndex = -1;
	for (int32 i = 0; i < INVENTORY_CAPACITY; i++) // 0, 1, 2
	{
		if (Inventory[i] == nullptr)
		{
			OutHasEmptySlot = true;
			ChangableIndex = i;
			break;
		}
	}
	if (ChangableIndex == -1) // Inventory is full!
	{
		OutHasEmptySlot = false;
		ChangableIndex = EquippedWeapon->SlotIndex;
		if (ChangableIndex == DefaultSlotIndex) // if DefaultWeapon is equipped
		{
			if (DefaultSlotIndex == 0)
			{
				ChangableIndex = 1;
			}
			else
			{
				ChangableIndex = 0;
			}
		}
	}

	return ChangableIndex;
}

void AShooterCharacter::HighlightInventorySlot()
{
	bool bHasEmptySlot;
	const int32 ChangableSlot{ GetChangableInventorySlot(bHasEmptySlot) };
	HighlightIconDelegate.Broadcast(ChangableSlot, true);
	HighlightedSlot = ChangableSlot;
}

EPhysicalSurface AShooterCharacter::GetSurfaceType()
{
	FHitResult HitResult;
	const FVector Start{ GetActorLocation() };
	const FVector End{ Start + FVector(0.f,0.f,-400.f) };
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		QueryParams);

	// PhysMaterial is a variable of TWeakObjectPtr type,
	// which is simillar as an weekptr as like a wrapper struct of ptr, 
	// so we can get the ptr value by using Get() method.
	// and also operator-> is overrided to dereference the ptr of the wrapper struct.
	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
}

void AShooterCharacter::EndStun()
{
	if (Health <= 0.f) return;

	CombatState = ECombatState::ECS_Unoccupied;
	if (bAimingButtonPressed)
	{
		Aim();
	}
}

void AShooterCharacter::Die_Implementation()
{
	if (CombatState == ECombatState::ECS_Dying) return;
	CombatState = ECombatState::ECS_Dying;


	AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		DisableInput(PC);
	}
}
void AShooterCharacter::FinishDeath()
{
	GetMesh()->bPauseAnims = true;
}

void AShooterCharacter::UnHighlightInventorySlot()
{
	HighlightIconDelegate.Broadcast(HighlightedSlot, false);
	HighlightedSlot = -1;
}

void AShooterCharacter::Stun()
{
	if (CombatState == ECombatState::ECS_Dying) return;
	CombatState = ECombatState::ECS_Stunned;
	AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
	}
}

void AShooterCharacter::OnRep_Health()
{
	if (UpdateHpDelegate.IsBound())
	{
		UpdateHpDelegate.Broadcast(Health, MaxHealth);
	}
}

void AShooterCharacter::OnRep_MaxHealth()
{
}

void AShooterCharacter::OnRep_CombatState()
{
	if (UpdateCombatStateDelegate.IsBound())
	{
		UpdateCombatStateDelegate.Execute(CombatState);
	}
}

int32 AShooterCharacter::GetInterpLocationIndex()
{
	int32 LowestIndex = 1;
	int32 LowestCount = INT_MAX;
	for (int32 i = 1; i < InterpLocations.Num(); i++)
	{
		if (InterpLocations[i].ItemCount < LowestCount)
		{
			LowestIndex = i;
			LowestCount = InterpLocations[i].ItemCount;
		}
	}

	return LowestIndex;
}

void AShooterCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount)
{
	if (Amount < -1 || Amount>1) return;

	if (InterpLocations.Num() >= Index)
	{
		InterpLocations[Index].ItemCount += Amount;
	}
}

void AShooterCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(
		PickupSoundTimer,
		this,
		&AShooterCharacter::ResetPickupSoundTimer,
		PickupSoundResetTime);
}

void AShooterCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer(
		EquipSoundTimer,
		this,
		&AShooterCharacter::ResetEquipSoundTimer,
		EquipSoundResetTime);
}

void AShooterCharacter::ResetPickupSoundTimer()
{
	bShouldPlayPickupSound = true;
}

void AShooterCharacter::ResetEquipSoundTimer()
{
	bShouldPlayEquipSound = true;
}

float AShooterCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	ReqGetPickupItem(Item);
}

void AShooterCharacter::ReqGetPickupItem_Implementation(AItem* Item)
{
	ResGetPickupItem(Item);
}

void AShooterCharacter::ResGetPickupItem_Implementation(AItem* Item)
{
	if (Item)
	{
		Item->PlayEquipSound();

		auto Weapon = Cast<AWeapon>(Item);
		if (Weapon)
		{
			bool bHasEmptySlot;
			int32 WeaponIndex = GetChangableInventorySlot(bHasEmptySlot);
			Weapon->SetOwningCharacter(this);
			Weapon->DisableGlowMaterial();
			Weapon->SetCanChangeCustomDepth(true);
			Weapon->DisableCustomDepth();
			if (bHasEmptySlot)
			{
				Weapon->SlotIndex = (WeaponIndex);
				Inventory[WeaponIndex] = Weapon;
				Weapon->SetItemState(EItemState::EIS_Pickedup);
			}
			else // Inventory is full! Swap with WeaponIndex
			{
				SwapWeapon(Weapon, WeaponIndex);
			}
		}

		auto Ammo = Cast<AAmmo>(Item);
		if (Ammo)
		{
			PickupAmmo(Ammo);
		}
	}
}
FInterpLocation AShooterCharacter::GetInterpLocation(int32 Index)
{
	if (Index < InterpLocations.Num())
	{
		return InterpLocations[Index];
	}

	return FInterpLocation();
}
