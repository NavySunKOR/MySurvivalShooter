 // Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include <Kismet/GameplayStatics.h>
#include "Inventory.h"

void UInventory::Init(APlayerCharacter* pPlayer)
{
	inventoryOwner = pPlayer;
	backpack = backpackType.GetDefaultObject();
	backpack->Init();
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
	if (backpack->HasItem(pItem))
	{
		APickableItem* picks = inventoryOwner->GetWorld()->SpawnActor<APickableItem>(pItem->meshToDrop);
		picks->SetActorLocation(inventoryOwner->GetActorLocation() + inventoryOwner->GetActorUpVector() * 50.f + inventoryOwner->GetActorForwardVector() * 50.f);
		//picks->itemInfo = pItem; //�� �Ŀ� itemInfo�� destroy �ؾߵǴ� ��찡 ����� �̰� �Ŵ������� ���۷��� �ؼ� �ٽ� �־�� ��.
		backpack->ActualRemoveItem(pItem);
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

bool UInventory::HasItem(UItemInfo* pItem)
{
	return backpack->HasItem(pItem);
}

int UInventory::GetAllPrimaryWeaponAmmo(FString pWeaponClassName)
{
	return 0;
}

int UInventory::GetAllSecondaryWeaponAmmo(FString pWeaponClassName)
{
	return 0;
}

UBackpack* UInventory::GetBackpack()
{
	return backpack;
}
