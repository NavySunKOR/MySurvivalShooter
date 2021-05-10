// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemArmor.h"
#include "ItemBodyArmor.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemBodyArmor : public UItemArmor
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<FJsonObject> GetJsonObject() override;
};
