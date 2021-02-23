// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.generated.h"

class APickableItem;
class UInventory;
class AFPPlayerController;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TARKOVCOPY_API UItemInfo : public UObject
{
	GENERATED_BODY()
public:
	//UItemInfo();
	//UItemInfo(float leftPos, float topPos, float width, float height);
	//virtual ~UItemInfo();

	UPROPERTY(EditAnywhere)
	int width;
	UPROPERTY(EditAnywhere)
	int height;

	int left;
	int top;

	UPROPERTY(EditAnywhere)
	int maxCapacity;
	UPROPERTY(EditAnywhere)
	int currentCapacity;
	UPROPERTY(EditAnywhere)
	UTexture2D* spriteToUse;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickableItem> meshToDrop;

	UInventory* refInventory;

	FSlateRect rect;

	virtual bool Use();
	virtual void DropItem(AFPPlayerController* pPlayerCon);
	virtual void InitRect(float pLeft, float pTop);
	UItemInfo operator=(UItemInfo &other);

};
