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
	ABaseGun* primaryWeapon;
	ABaseGun* secondaryWeapon;

	APlayerCharacter* inventoryOwner;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackpack> backpackType;
	UBackpack* backpack;

	void Init(APlayerCharacter* pPlayer);
	bool AddItemToInventory(UItemInfo* item);
	bool UseItem(UItemInfo* pItem);
	bool DropItem(UItemInfo* pItem);
	void RemoveItem(UItemInfo* pItem);
	bool HasItem(UItemInfo* pItem);

	//AboutWeapons

	int GetAllPrimaryWeaponAmmo(FString pWeaponClassName);
	int GetAllSecondaryWeaponAmmo(FString pWeaponClassName);

	UFUNCTION(BlueprintPure)
	UBackpack* GetBackpack();

};
