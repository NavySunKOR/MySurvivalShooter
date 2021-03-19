// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "ItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemWeapon : public UItemInfo
{
	GENERATED_BODY()
public:
	virtual void Use(AFPPlayerController* pPlayerCon) override; 
	virtual void DropItem(AFPPlayerController* pPlayerCon) override;
	virtual FString GetItemAmountString() const override;
	UPROPERTY(EditAnywhere)
	int currentMagazineAmmo;
	UPROPERTY(EditAnywhere)
	int maxMagazineAmmo;
	
};
