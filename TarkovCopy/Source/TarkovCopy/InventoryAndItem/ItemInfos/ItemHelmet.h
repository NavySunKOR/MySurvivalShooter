// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemArmor.h"
#include "ItemHelmet.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemHelmet : public UItemArmor
{
	GENERATED_BODY()
public:
	virtual void Use(AFPPlayerController* pPlayerCon) override; // Return : isEmpty
	virtual void DropItem(AFPPlayerController* pPlayerCon) override;
	virtual TSharedPtr<FJsonObject> GetJsonObject() override;

};
