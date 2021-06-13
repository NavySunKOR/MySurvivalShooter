// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UStaticMeshComponent;
class AHandGrenade;
class ACharacter;
class ABulletProjectile;
class ABaseGun;
class UItemInfo;
class UItemHelmet;
class UItemWeapon;
class UInventory;
class AFPPlayerController;
class ATarkovCopyGameModeBase;
class FSlateRect;

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
	UPROPERTY()
	TArray<AHandGrenade*> handGrenadePools;
	UPROPERTY()
	TArray<AHandGrenade*> flashGrenadePools;
	UPROPERTY()
	TArray<ABulletProjectile*> bulletProjectilePools;

	UPROPERTY()
	AHandGrenade* handGrenadePool;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called when take damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;



	UStaticMeshComponent* helmetMesh;


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

	//무기 관련
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

	//수류탄
	void ThrowGrenade();

	//섬광탄
	void ThrowFlashGrenade();


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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHandGrenade> handGrenadeOrigin;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHandGrenade> flashGrenadeOrigin;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> bulletProjectileOrigin;

	ATarkovCopyGameModeBase* gameMode;


public:	
	UPROPERTY()
	UInventory* inventory;
	AFPPlayerController* playerController;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void BeginDestroy();

	float HealPlayer(float pHealAmount);
	void Tilting(float pValue);

	//아이템 이동 관련
	bool PickupItem(UItemInfo* pItemInfo);
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect);
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	//무기 관련
	void AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin,UItemWeapon* pItemWeapon);
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon);
	void RemovePrimary();
	void RemoveSecondary();
	void FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir);

	//방어구 관련
	void AddHelmet(UItemHelmet* pHelmetInfo);
	void RemoveHelmet(UItemHelmet* pHelmetInfo);

	//저장 관련
	void SaveEquipments();


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
