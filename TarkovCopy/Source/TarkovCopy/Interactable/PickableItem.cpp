// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"

void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	if (initItem)
		Initalize(initItem);
}

void APickableItem::Initalize(TSubclassOf<UItemInfo> pItemInfo)
{
	//itemInfo = pItemInfo.GetDefaultObject();
	itemInfo = DuplicateObject(pItemInfo.GetDefaultObject(), this);
	
	
	/*itemInfo = NewObject<UItemInfo>(this, pItemInfo);*/
	//UItemInfo* origin = pItemInfo.GetDefaultObject();
	//itemInfo->width = origin->width;
	//itemInfo->height = origin->height;
	//itemInfo->spriteToUse = origin->spriteToUse;
	//itemInfo->meshToDrop = origin->meshToDrop;
	//itemInfo->weaponSubclass = origin->weaponSubclass;
	//itemInfo->maxCapacity = origin->maxCapacity;
	//itemInfo->currentCapacity = origin->currentCapacity;
}

void APickableItem::Interact()
{
	Super::Interact();
	
	if (playerCharacter->PickupItem(itemInfo))
		SetActorHiddenInGame(true);
		//Destroy();
	else
		UE_LOG(LogTemp, Warning, TEXT("Failed to pickup"));
}