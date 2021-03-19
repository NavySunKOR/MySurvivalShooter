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
	if (backpack->HasItem(pItem))
	{
		APickableItem* picks = inventoryOwner->GetWorld()->SpawnActor<APickableItem>(pItem->meshToDrop); //TODO:���߿� SETVisibleOff�ߴ� �������� �ڸ��� �����ϰ� visible�� ų��
		picks->itemInfo = pItem; // �̹� �Ծ��� �������� �ٽ� ���س��°��̹Ƿ� ���� ���ο� ������ �ƴϰ� ���� ������ ������ �����Ѵ�.
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

void UInventory::StartMoveItemPos(UItemInfo* pItemInfo)
{
	backpack->RemoveItemPosition(pItemInfo);
}

bool UInventory::CanItemMoveTo(FSlateRect pIntSlateRect)
{
	return backpack->HasEmptySpace(pIntSlateRect);
}

void UInventory::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	//remove item origin position
	backpack->RemoveItemPosition(pItemInfo);

	pItemInfo->left = pIntSlateRect.Left;
	pItemInfo->top = pIntSlateRect.Top;

	backpack->MoveItemPosition(pItemInfo);

	//move to pIntSlateRect

}

void UInventory::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	backpack->MoveItemPosition(pItemInfo);
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
