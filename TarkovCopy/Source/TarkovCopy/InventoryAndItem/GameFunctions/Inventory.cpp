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
		//picks->itemInfo = pItem; //추 후에 itemInfo를 destroy 해야되는 경우가 생기면 이걸 매니저에서 레퍼런싱 해서 다시 넣어야 됨.
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
