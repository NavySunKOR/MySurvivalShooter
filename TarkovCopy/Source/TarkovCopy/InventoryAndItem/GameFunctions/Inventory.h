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

	int GetAllPrimaryWeaponAmmo(FString pWeaponClassName);
	int GetAllSecondaryWeaponAmmo(FString pWeaponClassName);

	void UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);
	void UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);

	void UpdateAndCleanupBackpack();

	UFUNCTION(BlueprintPure)
	UBackpack* GetBackpack();
};
