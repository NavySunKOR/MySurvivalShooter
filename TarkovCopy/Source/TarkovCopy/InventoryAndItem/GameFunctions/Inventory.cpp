 // Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include <Kismet/GameplayStatics.h>

void UInventory::Init(APlayerCharacter* pPlayer)
{
	GetWorld()->ForceGarbageCollection(true);
	inventoryOwner = pPlayer;
	if (backpack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Huh????? %d"), backpack)
		backpack->ConditionalBeginDestroy();
		backpack = nullptr;
		GetWorld()->ForceGarbageCollection(true);
		backpack = backpackType->GetDefaultObject<UBackpack>();
		backpack->Init();
	}
	else
	{
		backpack = backpackType->GetDefaultObject<UBackpack>();
		backpack->Init();
	}
}

bool UInventory::AddItemToInventory(UItemInfo* item)
{
	return backpack->AddItem(item,this);
}

bool UInventory::UseItem(UItemInfo* pItem)
{
	if (backpack->HasItem(pItem))
	{
		if (pItem->isConsumable)
		{
			if (backpack->UseItem(pItem))
				return true;
			else 
				return false;
		}
		return false;
	}
	else
	{
		return true;
	}
}

bool UInventory::DropItem(UItemInfo* pItem)
{
	//TODO:Primary Weapon, Secondary Weapon �� �����°� ������ ó�� �� ��
	if (backpack->HasItem(pItem) || primaryWeapon == (UItemWeapon*)pItem || secondaryWeapon == (UItemWeapon*)pItem)
	{
		APickableItem* picks = inventoryOwner->GetWorld()->SpawnActor<APickableItem>(pItem->meshToDrop); //TODO:���߿� SETVisibleOff�ߴ� �������� �ڸ��� �����ϰ� visible�� ų��
		picks->itemInfo = pItem; // �̹� �Ծ��� �������� �ٽ� ���س��°��̹Ƿ� ���� ���ο� ������ �ƴϰ� ���� ������ ������ �����Ѵ�.
		picks->SetActorLocation(inventoryOwner->GetActorLocation() + inventoryOwner->GetActorUpVector() * 50.f + inventoryOwner->GetActorForwardVector() * 50.f);
		if(backpack->HasItem(pItem))
			backpack->ActualRemoveItem(pItem);
		else if(primaryWeapon == (UItemWeapon*)pItem)
			primaryWeapon = nullptr;
		else
			secondaryWeapon = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

void UInventory::RemoveItem(UItemInfo* pItem)
{
	backpack->ActualRemoveItem(pItem);
}

bool UInventory::HasItem(UItemInfo* pItem) const
{
	return (backpack->HasItem(pItem) || primaryWeapon == (UItemWeapon*)pItem || secondaryWeapon == (UItemWeapon*)pItem);
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

	pItemInfo->left = pIntSlateRect.Left;
	pItemInfo->top = pIntSlateRect.Top;

	UE_LOG(LogTemp, Warning, TEXT("is stil alive? : %d"), pItemInfo);

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

void UInventory::UpdateAndCleanupBackpack()
{
	backpack->UpdateAndCleanupBackpack();
}

UBackpack* UInventory::GetBackpack()
{
	return backpack;
}
