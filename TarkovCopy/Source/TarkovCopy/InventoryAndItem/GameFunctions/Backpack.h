// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "Backpack.generated.h"

class UInventory;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TARKOVCOPY_API UBackpack : public UObject
{
	GENERATED_BODY()
private:
	bool** invenVisualize;
	TArray<UItemInfo*> itemContainers;
	std::tuple<bool, int, int> HasEmptySpaceWidthAxis(UItemInfo* pItemInfo);
	std::tuple<bool, int, int> HasEmptySpaceHeightAxis(UItemInfo* pItemInfo);
	void UpdateInvenVisualize(UItemInfo* pItemInfo);
	void RemoveInvenVisualize(UItemInfo* pItemInfo);

public:
	UPROPERTY(EditAnywhere)
	int capacityWidth;
	UPROPERTY(EditAnywhere)
	int capacityHeight;

	void Init();
	std::tuple<bool, int, int> HasEmptySpace(UItemInfo* pItemInfo);
	bool IsIntersected(UItemInfo* pItemInfo);
	bool UseItem(UItemInfo* pItemInfo);//return : isEmpty;
	bool AddItem(UItemInfo* pItemInfo,UInventory* pInventory);
	bool HasItem(UItemInfo* pItemInfo);
	void ActualRemoveItem(UItemInfo* pItemInfo);
	int GetAllPrimaryWeaponAmmo(FString pWeaponClassName);
	int GetAllSecondaryWeaponAmmo(FString pWeaponClassName);
	void UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);
	void UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName);
	UItemInfo* GetItemReference(UItemInfo* pItemPtr);
	FVector2D GetBackpackSize();
};
