// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "ItemArmor.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemArmor : public UItemInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float curDurability;
	UPROPERTY(EditAnywhere)
	float maxDurability;
	UPROPERTY(EditAnywhere)
	float damageDecreaseAmount;
	virtual FString GetItemAmountString() const override;
	virtual TSharedPtr<FJsonObject> GetJsonObject() override;
	virtual void SetJsonObject(TSharedPtr<FJsonObject> pJsonObject) override;
};
