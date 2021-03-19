// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Blueprint/UserWidget.h>
#include <DrawDebugHelpers.h>
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
UCLASS()
class TARKOVCOPY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

private: 
	float moveVerticalValue = 0.f;
	float moveHorizontalValue = 0.f;
	float maxWalkValue = 0.f;
	bool isFired = false; // for semi auto mode only
	bool isFirePressed = false; 
	USpringArmComponent* springArm;
	FVector springArmOrigin;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AFPPlayerController* playerController;
	bool isSprinting = false;
	bool isCrouch = false;

	float checkCloseToWallTimer = 0.f;
	float checkCloseToWallInterval = 1.f;

	bool isCloseToWall = false;

	void MoveVertical(float pValue);
	void MoveHorizontal(float pValue);
	void RotateHorizontal(float pValue);
	void RotateVertical(float pValue);
	void SetSprinting();
	void SetWalking();
	void SetCrouch();
	void SetStanding();
	void CheckCloseToWall();

	void EquipPrimary();
	void EquipSecondary();
	void FireWeapon();
	void FireUpWeapon();
	void SetADSWeapon();
	void SetHipfireWeapon();
	void ReloadWeapon();
	void SetWeaponSelector();
	void ActualFireWeapon();

	void Interact();
	void InspectWeapon();
	void Inventory();



	ABaseGun* currentActiveGun;
	ABaseGun* primaryWeapon;
	ABaseGun* secondaryWeapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> m416Origin;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> m9Origin;

	UPROPERTY(EditAnywhere)
	int maxHp = 100;
	UPROPERTY(EditAnywhere)
	int curHp;


	UPROPERTY(EditAnywhere)
	float sprintingSpeed;
	UPROPERTY(EditAnywhere)
	float walkingSpeed;
	UPROPERTY(EditAnywhere)
	float adsWalkingSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventory> inventoryOrigin;

	class ATarkovCopyGameModeBase* gameMode;


public:	
	UPROPERTY()
	UInventory* inventory;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void BeginDestroy();

	float HealPlayer(float pHealAmount);
	void TookDamage(float damage, FHitResult pHitParts);
	void Tilting(float pValue);


	bool PickupItem(UItemInfo* pItemInfo);
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect);
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	void AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin,UItemWeapon* pItemWeapon);
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon);
	void RemovePrimary();
	void RemoveSecondary();

//BlueprintPureOnly
protected:
	UFUNCTION(BlueprintPure)
    bool IsWeaponEquiped();
	UFUNCTION(BlueprintPure)
	bool IsAds();
	UFUNCTION(BlueprintPure)
	bool IsFiring();
	UFUNCTION(BlueprintPure)
	bool IsReloading();
	UFUNCTION(BlueprintPure)
	bool IsSprinting();
	UFUNCTION(BlueprintPure)
	bool IsWalking();
	UFUNCTION(BlueprintPure)
	int GetWeaponCode();
	UFUNCTION(BlueprintPure)
	bool IsEmptyMagazine();
	UFUNCTION(BlueprintPure)
	bool IsCloseToWall();
	//Shotgun only
	UFUNCTION(BlueprintPure)
	bool IsShotgun();
	UFUNCTION(BlueprintPure)
	int GetReloadState();

};
