// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerComponent.h"
#include "PlayerEquipmentComponent.generated.h"


/*

	����ü Ǯ
	�÷��̾�� ��� ����ü Ǯ�� �����ش�. ������ ����ü Ǯ�� ����ӵ��� ������ �޴µ�,
	���⺰�� ����� �ᱹ ���� ����ӵ��� �°Բ� Ǯ�� �����λ����ȴ�.
	��: ���ݼ��� �ִ� ����� 30�� Ǯ ����, ������ �ִ� 15�� ���� ��� �Ѵٰ� �����ϸ�
	ĳ���Ϳ� ������ �� : Ǯ 30��
	�ѿ� ������ �� : 45��
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TARKOVCOPY_API UPlayerEquipmentComponent : public UBasePlayerComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<AHandGrenade*> handGrenadePools;
	UPROPERTY()
	TArray<AHandGrenade*> flashGrenadePools;
	UPROPERTY()
	TArray<ABulletProjectile*> bulletProjectilePools;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHandGrenade> handGrenadeOrigin;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHandGrenade> flashGrenadeOrigin;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> bulletProjectileOrigin;

	ABaseGun* currentActiveGun;
	UPROPERTY()
	ABaseGun* primaryWeapon;
	UPROPERTY()
	ABaseGun* secondaryWeapon;

	bool isFired = false; // for semi auto mode only

public:
	void InitEquipmentComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter);
	bool AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon);
	bool AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon);
	void RemovePrimary();
	void RemoveSecondary();
	void EquipPrimary();
	void EquipSecondary();

	void FireWeapon();
	void UnlockFireWeapon();
	void ActualFireWeapon();
	void SetWeaponSelector();
	void SetADSWeapon();
	void SetHipfireWeapon();
	void InspectWeapon();


	bool CanReloadWeapon() const;
	int GetNeedAmmoForReload() const;
	void ActualReloadWeapon(int needAmmo);

	FORCEINLINE ABaseGun* GetCurrentEquipedWepaon() { return currentActiveGun; };


	void ThrowGrenade();
	void ThrowFlashGrenade();
	void FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir);

public:
	bool IsPrimaryWeaponEquiped();
	bool IsWeaponEquiped();
	bool IsAds();
	bool IsFiring();
	bool IsReloading();
	int GetWeaponCode();
	bool IsEmptyMagazine();
	bool IsShotgun();
	int GetReloadState();

};
