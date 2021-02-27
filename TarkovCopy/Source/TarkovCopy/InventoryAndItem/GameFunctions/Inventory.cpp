 // Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include <Kismet/GameplayStatics.h>
#include "Inventory.h"

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
	return backpack->GetAllPrimaryWeaponAmmo(pWeaponClassName);
}

int UInventory::GetAllSecondaryWeaponAmmo(FString pWeaponClassName)
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

void UInventory::UpdateAndCleanupBackpack()
{
	backpack->UpdateAndCleanupBackpack();
}

UBackpack* UInventory::GetBackpack()
{
	return backpack;
}
