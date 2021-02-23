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
	UItemInfo* foundItem = backpack->GetItemReference(pItem);
	return foundItem->Use();
}

bool UInventory::DropItem(UItemInfo* pItem)
{
	if (backpack->CanRemoveItem(pItem))
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

UBackpack* UInventory::GetBackpack()
{
	return backpack;
}
