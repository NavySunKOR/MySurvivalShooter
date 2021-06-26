#include "PlayerEquipmentComponent.h"

bool UPlayerEquipmentComponent::IsWeaponEquiped()
{
	return (currentActiveGun != nullptr);
}

bool UPlayerEquipmentComponent::IsAds()
{
	return (currentActiveGun && currentActiveGun->isAds);
}

bool UPlayerEquipmentComponent::IsPrimaryWeaponEquiped()
{
	return currentActiveGun == primaryWeapon;
}

int UPlayerEquipmentComponent::GetWeaponCode()
{
	return (currentActiveGun) ? currentActiveGun->itemCode : -1;
}

bool UPlayerEquipmentComponent::IsEmptyMagazine()
{
	return (currentActiveGun && currentActiveGun->curMagRounds <= 0);
}

bool UPlayerEquipmentComponent::IsShotgun()
{
	return (currentActiveGun && currentActiveGun->itemCode == 3);
}

int UPlayerEquipmentComponent::GetReloadState()
{
	return (currentActiveGun && currentActiveGun->itemCode == 3) ? currentActiveGun->reloadState : -1;
}

bool UPlayerEquipmentComponent::IsFiring()
{
	return (currentActiveGun && currentActiveGun->isFiring);
}

bool UPlayerEquipmentComponent::IsReloading()
{
	return (currentActiveGun && currentActiveGun->isReloading);
}