// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemWeapon.h"
#include "ItemM9.generated.h"

class AFPPlayerController;
class ABaseGun;
/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemM9 : public UItemWeapon
{
	GENERATED_BODY()
public:
	virtual void Use(AFPPlayerController* pPlayerCon) override; // Return : isConsumable
	virtual void DropItem(AFPPlayerController* pPlayerCon) override;
};
