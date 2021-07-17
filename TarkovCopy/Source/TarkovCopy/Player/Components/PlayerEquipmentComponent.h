// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerComponent.h"
#include "PlayerEquipmentComponent.generated.h"


/*

	투사체 풀
	플레이어에게 모든 투사체 풀을 몰아준다. 이유는 투사체 풀은 연사속도에 영향을 받는데,
	무기별로 만들면 결국 각자 연사속도에 맞게끔 풀이 별도로생성된다.
	예: 돌격소총 최대 연사시 30개 풀 생성, 권총은 최대 15개 까지 사용 한다고 가정하면
	캐릭터에 몰아줄 시 : 풀 30개
	총에 몰아줄 시 : 45개
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
