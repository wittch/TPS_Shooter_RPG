// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaterialIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* MuzzleFlash;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadShotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StunRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
		float StunDurationFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.5"))
		float BaseSpreadFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.3"))
		float BaseFireSpreadFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.15"))
		float ShootTimeDuration;
};

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void FinishMovingSlide();

	void UpdateSlideDisplacement();


private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/** Ammo count for this Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta=(AllowPrivateAccess = "true"))
	int32 Ammo;

	/** Maximum ammo that our weapon can hold */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	/** The type of weapon */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	/** The type of ammo for this weapon */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	/** FName for the Reload Montage Section */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	/** Name for the clip bone */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	/** True when moving the clip while reloading */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	/** Data table for weapon properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

	int32 PreviousMaterialIndex;

	/** Textures for the weapon crosshairs */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsRight;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsBottom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsTop;

	/** The speed at which automatic fire happens */
	

	/** Particle system spawned at the BarrelSocket */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	/** Particles spawned upon bullet impact */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* ImpactParticles;

	/** Smoke trail for bullets */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* BeamParticles;

	/** Sound played when the weapon is fired */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	/** Name of the bone to hide on the weapon mesh */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	FName BoneToHide;

	/** Amount that the slide is pushed back during pistol fire */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float SlideDisplacement;

	/** Curve for the slide displacement */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlideDisplacementCurve;

	/** Timer handle for updating SlideDisplacement */
	FTimerHandle SlideTimer;

	/** Time for displacing the slide during pistol fire */
	float SlideDisplacementTime;

	/** Speed Factor for Increase the slide speed */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float SlideSpeed;

	/** True when moving the pistol slide */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	bool bMovingSlide;

	/** Max distance for the slide on the pistol */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float MaxSlideDisplacement;

	/** Max rotation for the pistol recoil */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float MaxRecoilRotation;

	/** Amount that the pistol will rotate during pistol fire */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float RecoilRotation;

	/** True for auto gunfire */
	

	/** Amount of damage caused by a bullet */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;

	/** Amount of damage when a bullet hits the head */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float HeadShotDamage;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float StunRate;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float StunDurationFactor;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float BaseSpreadFactor;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float BaseFireSpreadFactor;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		float ShootTimeDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Deafult, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* OwningCharacter;

public:
	/** Adds an impulse to the Weapon */
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const {	return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	/** Called from Character class when firing weapon */
	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE void SetReloadMontageSection(FName Section) { ReloadMontageSection = Section; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetClipBoneName(FName Name) { ClipBoneName = Name; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE UParticleSystem* GetImpactParticles() const { return ImpactParticles; }
	FORCEINLINE UParticleSystem* GetBeamParticles() const { return BeamParticles; }
	

	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }
	FORCEINLINE float GetStunRate() const { return StunRate; }
	FORCEINLINE float GetStunDurationFactor() const { return StunDurationFactor; }
	FORCEINLINE float GetBaseSpreadFactor() const { return BaseSpreadFactor; }
	FORCEINLINE float GetBaseFireSpreadFactor() const { return BaseFireSpreadFactor; }
	FORCEINLINE float GetShootTimeDuration() const { return ShootTimeDuration; }
	FORCEINLINE AShooterCharacter* GetOwningCharacter() const { return OwningCharacter; }
	FORCEINLINE void SetOwningCharacter(AShooterCharacter* OwnCharacter) { this->OwningCharacter = OwnCharacter; }


	void StartSlideTimer();

	void ReloadAmmo(int32 Amount);

	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	bool ClipIsFull();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FORCEINLINE FName GetBoneToHide() const { return BoneToHide; }

	void SetWeaponType(EWeaponType Type);
	void SetWeaponData();

	UPROPERTY(VisibleAnyWhere, Replicated, BlueprintReadOnly, Category = DataTable)
	float AutoFireRate;

	UPROPERTY(VisibleAnyWhere, Replicated, BlueprintReadOnly, Category = "Weapon Properties")
		bool bAutomatic;

};
