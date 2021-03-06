 // Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Backpack.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include "TarkovCopy/Utils/JsonSaveAndLoader.h"
#include <Kismet/GameplayStatics.h>

void UInventory::Init(APlayerCharacter* pPlayer)
{
	inventoryOwner = pPlayer;
	if (backpack)
	{
		backpack->ConditionalBeginDestroy();
		backpack = nullptr;
		backpack = backpackType->GetDefaultObject<UBackpack>();
		backpack->Init(this);
	}
	else
	{
		backpack = backpackType->GetDefaultObject<UBackpack>();
		backpack->Init(this);
	}
}

bool UInventory::AddNewItemToInventory(UItemInfo* item)
{
	return backpack->AddNewItem(item,this);
}

bool UInventory::UseItem(UItemInfo* pItem)
{
	if (backpack->HasItem(pItem))
	{
		if (pItem->isConsumable)
		{
			return backpack->UseItem(pItem);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UInventory::DropItem(UItemInfo* pItem)
{
	//TODO:Primary Weapon, Secondary Weapon 을 버리는걸 별도로 처리 할 듯
	if (backpack->HasItem(pItem) || 
		primaryWeapon == (UItemWeapon*)pItem || 
		secondaryWeapon == (UItemWeapon*)pItem || 
		itemHelmet == (UItemHelmet*)pItem)
	{
		APickableItem* picks = inventoryOwner->GetWorld()->SpawnActor<APickableItem>(pItem->meshToDrop); //TODO:나중에 SETVisibleOff했던 아이템을 자리로 스폰하고 visible을 킬것
		picks->itemInfo = pItem; // 이미 먹었던 아이템을 다시 토해내는것이므로 완전 새로운 스폰이 아니고 일정 정보는 가지고 가야한다.
		picks->SetActorLocation(inventoryOwner->GetActorLocation() + inventoryOwner->GetActorUpVector() * 50.f + inventoryOwner->GetActorForwardVector() * 50.f);
		if (backpack->HasItem(pItem))
			backpack->DeleteItem(pItem);
		else if (primaryWeapon == (UItemWeapon*)pItem)
			primaryWeapon = nullptr;
		else if (secondaryWeapon == (UItemWeapon*)pItem)
			secondaryWeapon = nullptr;
		else
			itemHelmet = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

void UInventory::RemoveItem(UItemInfo* pItem)
{
	backpack->DeleteItem(pItem);
}

bool UInventory::HasItem(UItemInfo* pItem) const
{
	return (backpack->HasItem(pItem) || 
		primaryWeapon == (UItemWeapon*)pItem || 
		secondaryWeapon == (UItemWeapon*)pItem ||
		itemHelmet == (UItemHelmet*)pItem);
}

UItemInfo* UInventory::HasItemType(ItemType pItemType)
{
	return backpack->HasItemType(pItemType);
}

void UInventory::StartMoveItemPos(UItemInfo* pItemInfo)
{
	backpack->RemoveItemPosition(pItemInfo);
}

bool UInventory::CanItemMoveTo(FSlateRect pIntSlateRect) const
{
	return backpack->HasEmptySpace(pIntSlateRect);
}

void UInventory::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	//remove item origin position
	if (backpack->HasItem(pItemInfo))
	{
		backpack->RemoveItemPosition(pItemInfo);
	}
	else if (primaryWeapon == (UItemWeapon*)pItemInfo)
	{
		primaryWeapon = nullptr;
		backpack->AddItemContainerArray(pItemInfo);
	}
	else if (secondaryWeapon == (UItemWeapon*)pItemInfo)
	{
		secondaryWeapon = nullptr;
		backpack->AddItemContainerArray(pItemInfo);
	}
	else if (itemHelmet == (UItemHelmet*)pItemInfo)
	{
		itemHelmet = nullptr;
		backpack->AddItemContainerArray(pItemInfo);
	}

	pItemInfo->left = pIntSlateRect.Left;
	pItemInfo->top = pIntSlateRect.Top;

	backpack->MoveItemPosition(pItemInfo);

	//move to pIntSlateRect

}

void UInventory::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	backpack->MoveItemPosition(pItemInfo);
}
 
int UInventory::GetAllPrimaryWeaponAmmo(FString pWeaponClassName) const
{
	return backpack->GetAllPrimaryWeaponAmmo(pWeaponClassName);
}

int UInventory::GetAllSecondaryWeaponAmmo(FString pWeaponClassName) const
{
	return backpack->GetAllSecondaryWeaponAmmo(pWeaponClassName);
}

void UInventory::UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName)
{
	backpack->UsePrimaryWeaponAmmo(pUseAmmo, pWeaponClassName);
	
}

void UInventory::UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName)
{
	backpack->UseSecondaryWeaponAmmo(pUseAmmo, pWeaponClassName);
}

void UInventory::SetPrimaryWeaponItem(UItemWeapon* pItemWeapon)
{
	primaryWeapon = pItemWeapon;
	RemoveItem((UItemInfo*)pItemWeapon);
}

void UInventory::SetSecondaryWeaponItem(UItemWeapon* pItemWeapon)
{
	secondaryWeapon = pItemWeapon;
	RemoveItem((UItemInfo*)pItemWeapon);
}

void UInventory::EquipHelmet(UItemHelmet* pItemHelmet)
{
	itemHelmet = pItemHelmet;
	RemoveItem((UItemInfo*)pItemHelmet);
}

void UInventory::UnequipHelmet()
{
	itemHelmet = nullptr;
}

UItemHelmet* UInventory::GetEquippedHelmet()
{
	return itemHelmet;
}

void UInventory::UpdateAndCleanupBackpack()
{
	backpack->UpdateAndCleanupBackpack();
}

UBackpack* UInventory::GetBackpack()
{
	return backpack;
}

void UInventory::SaveEquipments()
{
	JsonSaveAndLoader::SaveEquipments(primaryWeapon, secondaryWeapon, itemHelmet);
}
