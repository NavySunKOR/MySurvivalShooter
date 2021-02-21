// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseGun.generated.h"

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
	FVector thirdPersonScale;
	FVector thirdPersonPosition;
	FRotator thirdPersonRotation;
	
	UPROPERTY(EditAnywhere)
	float rpm;
	
	float fireInterval;
	float fireTimer = 0.f;

	float reloadInterval;
	float reloadTimer = 0.f;

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

	UPROPERTY(EditAnywhere)
	UParticleSystem* hitTerrain;

	UPROPERTY(EditAnywhere)
	UAnimBlueprint* fppAnimBlueprints;
	UPROPERTY(EditAnywhere)
	UAnimBlueprint* tppAnimBlueprints;


	USkeletalMeshComponent* parentMesh;
	USceneComponent* magazineComponents;
	USceneComponent* attachmentComponents;
	USceneComponent* scopeComponents;
	USceneComponent* weaponComponents;
	USceneComponent* sliderComponents;


//TempValues
protected:
	int tempInsertMag;


};
