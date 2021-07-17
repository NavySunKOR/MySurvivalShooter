// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryComponent.h"
#include "TarkovCopy/Utils/JsonSaveAndLoader.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Backpack.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemHelmet.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UPlayerInventoryComponent::ReduceHelmetDurability(float& pDamageRef)
{
	if ((Inventory->GetEquippedHelmet() != nullptr && 
		Inventory->GetEquippedHelmet()->curDurability >= Inventory->GetEquippedHelmet()->damageDecreaseAmount))
	{
		pDamageRef -= Inventory->GetEquippedHelmet()->damageDecreaseAmount;
		Inventory->GetEquippedHelmet()->curDurability -= Inventory->GetEquippedHelmet()->damageDecreaseAmount;
	}
}

void UPlayerInventoryComponent::InitInventoryComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
{
	Super::Init(pPlayerController, pPlayerCharacter);

	//TODO:나중에 인벤토리 초기화 고칠것
	Inventory = nullptr;
	Inventory = NewObject<UInventory>(GetWorld(), InventoryOrigin);
	Inventory->Init(PlayerCharacter);


	TArray<UItemInfo*> tempContainers = JsonSaveAndLoader::LoadBackpackItemContainers(GetWorld());
	if (tempContainers.Num() > 0)
	{
		for (int i = 0; i < tempContainers.Num(); i++)
		{
			if (tempContainers[i])
			{
				Inventory->AddNewItemToInventory(tempContainers[i]);
				PlayerController->AddItem(Inventory->GetBackpack()->GetItemContainers()[Inventory->GetBackpack()->GetItemContainers().Num() - 1], Inventory);
			}
		}
	}

	if (PlayerController != nullptr)
		PlayerController->InitInvenotry();
}

void UPlayerInventoryComponent::StartMoveItemPos(UItemInfo* pItemInfo)
{
	Inventory->StartMoveItemPos(pItemInfo);
}

bool UPlayerInventoryComponent::CanItemMoveTo(FSlateRect pIntSlateRect) const
{
	return Inventory->CanItemMoveTo(pIntSlateRect);
}

void UPlayerInventoryComponent::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	Inventory->MoveItemTo(pItemInfo, pIntSlateRect);
}

void UPlayerInventoryComponent::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	Inventory->FailedToMoveItemPos(pItemInfo);
}
void UPlayerInventoryComponent::RemoveItem(UItemInfo* pItemInfo)
{
	Inventory->RemoveItem(pItemInfo);
}

bool UPlayerInventoryComponent::PickupItem(UItemInfo* pItemInfo) const
{
	bool isItemAdded = Inventory->AddNewItemToInventory(pItemInfo);
	if (isItemAdded && PlayerController != nullptr)
	{
		PlayerController->AddItem(pItemInfo, Inventory);
	}

	return isItemAdded;
}

void UPlayerInventoryComponent::AddHelmet(UItemHelmet* pHelmetInfo)
{
	Inventory->EquipHelmet(pHelmetInfo);
}

void UPlayerInventoryComponent::RemoveHelmet()
{
	Inventory->UnequipHelmet();
}

void UPlayerInventoryComponent::SaveEquipments()
{
	Inventory->SaveEquipments();
}

bool UPlayerInventoryComponent::IsUsingInventory() const
{
	return Inventory->isUsingInventory;
}

bool UPlayerInventoryComponent::HaveEnoughAmmoForReload(FString pWeaponName, bool isPrimary, int& pNeeded) const
{

	int ownedAmmo = 0;

	ownedAmmo = (isPrimary) ?
		Inventory->GetAllPrimaryWeaponAmmo(pWeaponName) :
		Inventory->GetAllSecondaryWeaponAmmo(pWeaponName);
	Inventory->isUsingInventory = true;

	if (ownedAmmo == 0)
	{
		Inventory->isUsingInventory = false;
		return false;
	}

	if (pNeeded > ownedAmmo)
	{
		pNeeded = ownedAmmo;
	}

	return true;
}

void UPlayerInventoryComponent::UsePrimaryAmmo(int pAmount, FString pWeaponName)
{
	Inventory->UsePrimaryWeaponAmmo(pAmount, pWeaponName);
	Inventory->UpdateAndCleanupBackpack();
	Inventory->isUsingInventory = false;
}

void UPlayerInventoryComponent::UseSecondaryAmmo(int pAmount, FString pWeaponName)
{
	Inventory->UseSecondaryWeaponAmmo(pAmount, pWeaponName);
	Inventory->UpdateAndCleanupBackpack();
	Inventory->isUsingInventory = false;
}

bool UPlayerInventoryComponent::HasItemType(UItemInfo* pItemReference,ItemType pItemType) const
{
	UItemInfo* itemReference = Inventory->HasItemType(pItemType);
	pItemReference = itemReference;

	if (itemReference == nullptr)
	{
		return false;
	}


	return true;
}

void UPlayerInventoryComponent::UseItem(UItemInfo* pItemInfo)
{
	Inventory->UseItem(pItemInfo);
	Inventory->UpdateAndCleanupBackpack();
	PlayerController->UpdateInventoryUI();
}
