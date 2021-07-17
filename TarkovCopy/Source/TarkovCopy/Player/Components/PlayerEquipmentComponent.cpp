// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEquipmentComponent.h"
#include "TarkovCopy/Weapons/FlashGrenade.h"
#include "TarkovCopy/Weapons/BulletProjectile.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemHelmet.h"

void UPlayerEquipmentComponent::ThrowGrenade()
{
	bool isThrowing = false;
	for (int i = 0; i < handGrenadePools.Num(); i++)
	{
		if (handGrenadePools[i] && !handGrenadePools[i]->IsActive())
		{
			//Add Physics power
			handGrenadePools[i]->ThrowGrenade(PlayerCharacter->GetActorForwardVector(), PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * 100.f + PlayerCharacter->GetActorForwardVector() * 100.f);
			isThrowing = true;
			break;
		}
	}

	if (!isThrowing)
	{
		AHandGrenade* hand = GetWorld()->SpawnActor<AHandGrenade>(handGrenadeOrigin);
		hand->SetInstigator(PlayerCharacter);
		hand->SetOwner(PlayerCharacter);
		handGrenadePools.Add(hand);
		handGrenadePools[handGrenadePools.Num() - 1]->ReactivateGrenade();
		//Add Physics power
		handGrenadePools[handGrenadePools.Num() - 1]->SetActorLocation(PlayerCharacter->GetActorLocation());
		handGrenadePools[handGrenadePools.Num() - 1]->ThrowGrenade(PlayerCharacter->GetActorForwardVector(), PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * 100.f + PlayerCharacter->GetActorForwardVector() * 100.f);
		isThrowing = true;
	}
}

void UPlayerEquipmentComponent::SetWeaponSelector()
{
	if (currentActiveGun && currentActiveGun->isCanAutoFire)
	{
		currentActiveGun->isAutoFire = !currentActiveGun->isAutoFire;
	}
}


bool UPlayerEquipmentComponent::CanReloadWeapon()const
{
	return currentActiveGun && !currentActiveGun->isReloading;
}

int UPlayerEquipmentComponent::GetNeedAmmoForReload() const
{
	return currentActiveGun->maximumMagRounds - currentActiveGun->curMagRounds;
}

void UPlayerEquipmentComponent::ActualReloadWeapon(int needAmmo)
{
	currentActiveGun->Reload(needAmmo);
}

void UPlayerEquipmentComponent::ThrowFlashGrenade()
{
	bool isThrowing = false;

	for (int i = 0; i < flashGrenadePools.Num(); i++)
	{
		if (flashGrenadePools[i] && !flashGrenadePools[i]->IsActive())
		{
			//Add Physics power
			flashGrenadePools[i]->ThrowGrenade(PlayerCharacter->GetActorForwardVector(), PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * 100.f + PlayerCharacter->GetActorForwardVector() * 100.f);
			isThrowing = true;
			break;
		}
	}

	if (!isThrowing)
	{
		AFlashGrenade* flash = GetWorld()->SpawnActor<AFlashGrenade>(flashGrenadeOrigin);

		flashGrenadePools.Add(flash);
		flashGrenadePools[flashGrenadePools.Num() - 1]->SetInstigator(PlayerCharacter);
		flashGrenadePools[flashGrenadePools.Num() - 1]->SetOwner(PlayerCharacter);
		//Add Physics power
		flashGrenadePools[flashGrenadePools.Num() - 1]->ThrowGrenade(PlayerCharacter->GetActorForwardVector(), PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorUpVector() * 100.f + PlayerCharacter->GetActorForwardVector() * 100.f);
		isThrowing = true;
	}
}


void UPlayerEquipmentComponent::FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir)
{
	bool isProjectileFired = false;
	for (int i = 0; i < bulletProjectilePools.Num(); i++)
	{
		if (bulletProjectilePools[i] && !bulletProjectilePools[i]->IsFired())
		{
			bulletProjectilePools[i]->ReactivateProjectile(pDamage, pVelocity, pMass, Cast<APawn>(PlayerCharacter), pShootDir);
			bulletProjectilePools[i]->LaunchProjectile();
			bulletProjectilePools[i]->SetActorLocation(pFireStartPos);
			isProjectileFired = true;
			break;
		}
	}


	if (!isProjectileFired)
	{
		ABulletProjectile* bullet = GetWorld()->SpawnActor<ABulletProjectile>(bulletProjectileOrigin, pFireStartPos, pShootDir.Rotation());
		bullet->SetInstigator(PlayerCharacter);
		bullet->SetOwner(PlayerCharacter);
		bulletProjectilePools.Add(bullet);
		//총알의 주인과 선동자 설정 - 주인을 나중에 무기로 바꿀려면 풀을 무기에서 생성하게끔 기능 자체를 옮겨줘야한다. - 특히 멀티 플레이어에서 잘 지켜보도록.
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->ReactivateProjectile(pDamage, pVelocity, pMass, Cast<APawn>(PlayerCharacter), pShootDir);
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->LaunchProjectile();
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->SetActorLocation(pFireStartPos);
		isProjectileFired = true;
	}
}

void UPlayerEquipmentComponent::InitEquipmentComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
{
	Super::Init(pPlayerController, pPlayerCharacter);
}

bool UPlayerEquipmentComponent::AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	if (primaryWeapon != nullptr)
		return false;
	primaryWeapon = GetWorld()->SpawnActor<ABaseGun>(pWeaponOrigin);
	if (primaryWeapon != nullptr)
	{
		primaryWeapon->SetInfo(pItemWeapon);
		primaryWeapon->SetParentMeshFPP(PlayerCharacter->GetMesh());
		primaryWeapon->AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
		primaryWeapon->SetOwner(PlayerCharacter);
		primaryWeapon->weaponOwnerCharacter = PlayerCharacter;
	}
	EquipPrimary();

	return true;
}

bool UPlayerEquipmentComponent::AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	if (secondaryWeapon != nullptr)
		return false;
	secondaryWeapon = GetWorld()->SpawnActor<ABaseGun>(pWeaponOrigin);
	if (secondaryWeapon != nullptr)
	{
		secondaryWeapon->SetInfo(pItemWeapon);
		secondaryWeapon->SetParentMeshFPP(PlayerCharacter->GetMesh());
		secondaryWeapon->AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
		secondaryWeapon->SetOwner(PlayerCharacter);
		secondaryWeapon->weaponOwnerCharacter = PlayerCharacter;
	}
	EquipSecondary();

	return true;
}


void UPlayerEquipmentComponent::RemovePrimary()
{
	if (currentActiveGun == primaryWeapon)
		currentActiveGun = nullptr;
	if (primaryWeapon)
		primaryWeapon->Destroy();
	primaryWeapon = nullptr;

	if (secondaryWeapon)
	{
		EquipSecondary();
	}
}

void UPlayerEquipmentComponent::RemoveSecondary()
{
	if (currentActiveGun == secondaryWeapon)
		currentActiveGun = nullptr;
	if (secondaryWeapon)
		secondaryWeapon->Destroy();
	secondaryWeapon = nullptr;

	if (primaryWeapon)
	{
		EquipPrimary();
	}
}


void UPlayerEquipmentComponent::EquipPrimary()
{
	if (primaryWeapon)
	{
		primaryWeapon->SetActorHiddenInGame(false);
		primaryWeapon->SetActorTickEnabled(true);
		currentActiveGun = primaryWeapon;
		currentActiveGun->EquipWeapon();
		if (secondaryWeapon)
		{
			secondaryWeapon->SetActorHiddenInGame(true);
			secondaryWeapon->SetActorTickEnabled(false);
		}
	}
}

void UPlayerEquipmentComponent::EquipSecondary()
{
	if (secondaryWeapon)
	{
		secondaryWeapon->SetActorHiddenInGame(false);
		secondaryWeapon->SetActorTickEnabled(true);
		currentActiveGun = secondaryWeapon;
		currentActiveGun->EquipWeapon();
		if (primaryWeapon)
		{
			primaryWeapon->SetActorHiddenInGame(true);
			primaryWeapon->SetActorTickEnabled(false);
		}
	}
}

void UPlayerEquipmentComponent::SetADSWeapon()
{
	if (currentActiveGun)
	{
		currentActiveGun->SetADS();
	}
}

void UPlayerEquipmentComponent::SetHipfireWeapon()
{
	if (currentActiveGun)
	{
		currentActiveGun->SetHipfire();
	}
}

void UPlayerEquipmentComponent::FireWeapon()
{
	if (currentActiveGun == nullptr)
		return;
	if (currentActiveGun->CanFireWeapon()) 
	{
		if(!isFired)
		{
			if (currentActiveGun->isAutoFire)
			{
				ActualFireWeapon();
			}
			else
			{
				isFired = true;
				ActualFireWeapon();
			}
		}
	}
	else if ((!IsFiring()) && (!IsReloading()) && IsEmptyMagazine()) //자동 반자동 상관없이 탄창이 비었으면 Dry fire을 함.
	{
		if (!isFired)
		{
			isFired = true;
			currentActiveGun->EmptyFireWeapon();
		}
	}
}

void UPlayerEquipmentComponent::UnlockFireWeapon()
{
	isFired = false;
}

void UPlayerEquipmentComponent::ActualFireWeapon()
{
	FVector start;
	FRotator dir;

	currentActiveGun->UpdateMuzzleInfo();
	start = currentActiveGun->muzzleStart;
	if (IsAds())
	{
		dir = currentActiveGun->muzzleDir;
	}
	else
	{
		FVector viewpointPos;
		FRotator viewpointRotation;
		PlayerController->GetPlayerViewPoint(viewpointPos, viewpointRotation);
		dir = ((viewpointPos + (viewpointRotation.Vector() * 500.f)) - start).Rotation();
	}

	currentActiveGun->FireWeapon(start, dir);

	if (IsAds())
	{
		PlayerCharacter->AddControllerPitchInput(-currentActiveGun->adsBulletVerticalSpreadIncrement);
		PlayerCharacter->AddControllerYawInput(currentActiveGun->adsBulletHorizontalSpreadIncrement);
	}

}

void UPlayerEquipmentComponent::InspectWeapon()
{
	if (currentActiveGun && !IsReloading() && !IsAds() && !IsFiring())
	{
		currentActiveGun->InspectWeapon();
	}
}
