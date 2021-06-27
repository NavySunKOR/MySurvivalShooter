// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "BasePlayerComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
class UInventory;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TARKOVCOPY_API UPlayerInventoryComponent : public UBasePlayerComponent
{
	GENERATED_BODY()
protected: 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventory> InventoryOrigin;
	UPROPERTY()
	UInventory* Inventory;
public:
	void ReduceHelmetDurability(float& pDamageRef);
	virtual void Init(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter) override;
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect) const;
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);
	void RemoveItem(UItemInfo* pItemInfo);
	bool PickupItem(UItemInfo* pItemInfo) const;

	void AddHelmet(UItemHelmet* pHelmetInfo);
	void RemoveHelmet();
	void SaveEquipments();

	bool IsUsingInventory() const;
	bool HaveEnoughAmmoForReload(FString pWeaponName, bool isPrimary, int& pNeeded) const;
	void UsePrimaryAmmo(int pAmount,FString pWeaponName);
	void UseSecondaryAmmo(int pAmount, FString pWeaponName);

	bool HasItemType(UItemInfo* pItemReference, ItemType pItemType) const;
	void UseItem(UItemInfo* pItemInfo);


	FORCEINLINE UInventory* GetInventory() { return Inventory; };

};
