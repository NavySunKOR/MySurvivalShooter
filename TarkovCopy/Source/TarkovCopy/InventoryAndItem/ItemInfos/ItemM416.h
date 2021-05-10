// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemWeapon.h"
#include "ItemM416.generated.h"

/**
 * 
 */
class ABaseGun;
class AFPPlayerController;
UCLASS()
class TARKOVCOPY_API UItemM416 : public UItemWeapon
{
	GENERATED_BODY()
public:
	virtual void Use(AFPPlayerController* pPlayerCon) override; // Return : isConsumable
	virtual void DropItem(AFPPlayerController* pPlayerCon) override;
	virtual TSharedPtr<FJsonObject> GetJsonObject() override;
};
