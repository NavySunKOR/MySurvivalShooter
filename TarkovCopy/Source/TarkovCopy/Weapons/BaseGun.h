// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseGun.generated.h"

class UArrowComponent;
class APlayerCharacter;

UCLASS()
class TARKOVCOPY_API ABaseGun : public APawn
{
	GENERATED_BODY()
private:
	//UPROPERTY(VisibleAnywhere)
	//USceneComponent* root;

	//UPROPERTY(VisibleAnywhere)
	//USkeletalMeshComponent* gunMesh;


public:
	// Sets default values for this pawn's properties
	ABaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* parentMesh;
	USceneComponent* magazineComponents;
	USceneComponent* attachmentComponents;
	USceneComponent* scopeComponents;
	USceneComponent* weaponComponents;
	USceneComponent* sliderComponents;
	USceneComponent* hammerComponents;
	UArrowComponent* muzzleArrow;

	UPROPERTY(EditAnywhere)
	UParticleSystem* hitTerrain;
	UPROPERTY(EditAnywhere)
	UAnimBlueprint* fppAnimBlueprints;
	UPROPERTY(EditAnywhere)
	UAnimBlueprint* tppAnimBlueprints;

	UPROPERTY(EditAnywhere)
	UAnimMontage* emptyReload;
	UPROPERTY(EditAnywhere)
	UAnimMontage* tacticalReload;

public:	
	int itemCode;
	int reloadState; //use for shotgun
	bool isAds = false;
	bool isFiring = false;
	bool isReloading = false;
	int maximumMagRounds;
	int curMagRounds;
	float damage;
	float range;
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
	
	UPROPERTY(EditAnywhere)
	float rpm;

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;

	UPROPERTY(EditAnywhere)
	USoundBase* reloadSound;

	virtual bool CanFireWeapon();
	virtual void FireWeapon(FVector start, FRotator dir);
	virtual void Reload(int pInsertMagazine);
	virtual void SetADS();
	virtual void SetHipfire();

	virtual void SetParentMeshFPP(USkeletalMeshComponent* pMeshComp);
	virtual void SetParentMeshTPP(USkeletalMeshComponent* pMeshComp);
	virtual void EquipWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//BlueprintPures
protected: 
	UFUNCTION(BlueprintPure)
		bool IsReloading();
	UFUNCTION(BlueprintPure)
		bool IsFiring();
//TempValues
protected:
	int tempInsertMag;


};
