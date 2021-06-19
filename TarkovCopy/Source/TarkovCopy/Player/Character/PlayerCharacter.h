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
class UPlayerStatusComponent;
class UPlayerMovementComponent;

UCLASS()
class TARKOVCOPY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

private: 
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


	//Components
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPlayerStatusComponent* PlayerStatusComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPlayerMovementComponent* PlayerMovementComponent;


	//InputFunctions

	void SetInputSprint();
	void SetInputWalk();
	void SetInputCrouch();
	void SetInputUncrouch();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Called when take damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UStaticMeshComponent* helmetMesh;


	bool isCloseToWall = false;

	void MoveVertical(float pValue);
	void MoveHorizontal(float pValue);
	void RotateHorizontal(float pValue);
	void RotateVertical(float pValue);

	//���� ����
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

	//����ź
	void ThrowGrenade();

	//����ź
	void ThrowFlashGrenade();


	ABaseGun* currentActiveGun;
	ABaseGun* primaryWeapon;
	ABaseGun* secondaryWeapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> m416Origin;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> m9Origin;

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
	void Tilting(float pValue);

	//������ �̵� ����
	bool PickupItem(UItemInfo* pItemInfo);
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect);
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	//���� ����
	void AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin,UItemWeapon* pItemWeapon);
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon);
	void RemovePrimary();
	void RemoveSecondary();
	void FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir);
	FORCEINLINE ABaseGun* GetCurrentActiveGun() { return currentActiveGun; };

	//�� ����
	void AddHelmet(UItemHelmet* pHelmetInfo);
	void RemoveHelmet(UItemHelmet* pHelmetInfo);

	//���� ����
	void SaveEquipments();

	//���� ����
	void HealPlayer(float pHealAmount);

//BlueprintPureOnly
public:
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
