// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "ItemMedikit.generated.h"

class AFPPlayerController;
/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemMedikit : public UItemInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float healAmount;

	virtual void Use(AFPPlayerController* pPlayerCon) override;
};
