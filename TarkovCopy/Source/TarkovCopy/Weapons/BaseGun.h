// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseGun.generated.h"

class UArrowComponent;
class UItemInfo;
class APlayerCharacter;
class AAICharacter;

UCLASS()
class TARKOVCOPY_API ABaseGun : public APawn
{
	GENERATED_BODY()
private:
	bool isFPP;

public:
	// Sets default values for this pawn's properties
	ABaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float calcBulletSpreadRadius;

	USkeletalMeshComponent* parentMesh = nullptr;
	USceneComponent* magazineComponents = nullptr;
	USceneComponent* attachmentComponents = nullptr;
	USceneComponent* scopeComponents = nullptr;
	USceneComponent* weaponComponents = nullptr;
	USceneComponent* sliderComponents = nullptr;
	USceneComponent* hammerComponents = nullptr;
	UArrowComponent* muzzleArrow = nullptr;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* muzzleFireParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitTerrainParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitEnemyParticle;

	UPROPERTY()
	UAnimBlueprintGeneratedClass* fppAnimBlueprints;
	UPROPERTY()
	UAnimBlueprintGeneratedClass* tppAnimBlueprints;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* fireAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* emptyFireAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* aimFireAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* aimEmptyFireAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* emptyReloadAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* tacticalReloadAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* inspectWeaponAnim;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* equipWeaponAnim;


	UPROPERTY(EditAnywhere,Category="Sound")
	USoundBase* fireWeaponSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* emptyFireWeaponSound;
	UPROPERTY(EditAnywhere,Category="Sound")
	USoundBase* hitTerrainSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitEnemySound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* emptyReloadSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* tacticalReloadSound;

public:
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float rpm;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	int maximumMagRounds;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float damage;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float range;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	int itemCode;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float initBulletSpreadRadius;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float maxBulletSpreadRadius;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float bulletSpreadIncrement;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float bulletSpreadDecrementPerSecond;

	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float adsBulletHorizontalSpreadIncrement;
	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	float adsBulletVerticalSpreadIncrement;

	UPROPERTY(EditAnywhere, Category = "EditableStatus")
	bool isCanAutoFire;
	UPROPERTY()
	bool isAutoFire;


	int reloadState; //use for shotgun
	bool isAds = false;
	bool isFiring = false;
	bool isReloading = false;
	int curMagRounds;
	float fireInterval;
	float fireTimer = 0.f;
	float reloadInterval;
	float reloadTimer = 0.f;

	FVector thirdPersonScale;
	FVector thirdPersonPosition;
	FVector muzzleStart;
	FRotator thirdPersonRotation;
	FRotator muzzleDir;

	APlayerCharacter* weaponOwnerCharacter;
	AAICharacter* weaponOwnerAICharacter;
	

	virtual void UpdateMuzzleInfo();
	virtual bool CanFireWeapon();
	virtual void FireWeapon(FVector start, FRotator dir);
	virtual void EmptyFireWeapon();
	virtual void Reload(int pInsertMagazine);
	virtual void SetADS();
	virtual void SetHipfire();

	virtual void SetParentMeshFPP(USkeletalMeshComponent* pMeshComp);
	virtual void SetParentMeshTPP(USkeletalMeshComponent* pMeshComp);
	virtual void EquipWeapon();
	virtual void InspectWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//BlueprintPures
protected: 
	bool IsReloading();
	bool IsFiring();

//TempValues
protected:
	int tempInsertMag;


};
