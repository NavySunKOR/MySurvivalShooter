// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

/**
 * 
 */

class ABaseGun;
class UBackpack;
class UItemWeapon;
class UItemHelmet;

UCLASS(Blueprintable, BlueprintType)
class TARKOVCOPY_API UInventory : public UObject
{
	GENERATED_BODY()
private :
	UItemWeapon* primaryWeapon = nullptr;
	UItemWeapon* secondaryWeapon = nullptr;
	UItemHelmet* itemHelmet = nullptr;
	UPROPERTY()
	UBackpack* backpack = nullptr;

public:

	APlayerCharacter* inventoryOwner = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackpack> backpackType;

	void Init(APlayerCharacter* pPlayer);
	bool AddNewItemToInventory(UItemInfo* item);
	bool UseItem(UItemInfo* pItem);
	bool DropItem(UItemInfo* pItem);
	void RemoveItem(UItemInfo* pItem);
	bool HasItem(UItemInfo* pItem) const;
	UItemInfo* HasItemType(ItemType pItemType);

	//Drag And Drop
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect) const;
	void MoveItemTo(UItemInfo* pItemInfo,FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	//AboutWeapons

	bool isUsingInventory = false; // 인벤토리를 사용중인지 아닌지 접근을 제어하는 변수. 이걸로 멀티쓰레딩으로 인벤에 두 개 이상의 요소가 동시에 접근하는 것을 방지

	int GetAllPrimaryWeaponAmmo(FString pWeaponClassName) const;
	int GetAllSecondaryWeaponAmmo(FString pWeaponClassName)const;

	void UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);
	void UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);

	void SetPrimaryWeaponItem(UItemWeapon* pItemWeapon);
	void SetSecondaryWeaponItem(UItemWeapon* pItemWeapon);

	void EquipHelmet(UItemHelmet* pItemHelmet);
	void UnequipHelmet();
	UItemHelmet* GetEquippedHelmet();

	void UpdateAndCleanupBackpack();

	void SaveEquipments();

	UFUNCTION(BlueprintPure)
	UBackpack* GetBackpack();
};
