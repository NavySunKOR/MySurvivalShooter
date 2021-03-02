// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Backpack.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TARKOVCOPY_API UInventory : public UObject
{
	GENERATED_BODY()
public:
	ABaseGun* primaryWeapon = nullptr;
	ABaseGun* secondaryWeapon = nullptr;

	APlayerCharacter* inventoryOwner = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackpack> backpackType;
	UPROPERTY()
	UBackpack* backpack = nullptr;

	void Init(APlayerCharacter* pPlayer);
	bool AddItemToInventory(UItemInfo* item);
	bool UseItem(UItemInfo* pItem);
	bool DropItem(UItemInfo* pItem);
	void RemoveItem(UItemInfo* pItem);
	bool HasItem(UItemInfo* pItem);

	//AboutWeapons

	bool isUsingInventory = false; // 인벤토리를 사용중인지 아닌지 접근을 제어하는 변수. 이걸로 멀티쓰레딩으로 인벤에 두 개 이상의 요소가 동시에 접근하는 것을 방지

	int GetAllPrimaryWeaponAmmo(FString pWeaponClassName);
	int GetAllSecondaryWeaponAmmo(FString pWeaponClassName);

	void UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);
	void UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);

	void UpdateAndCleanupBackpack();

	UFUNCTION(BlueprintPure)
	UBackpack* GetBackpack();
};
