// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableItem.h"
#include <Components/BoxComponent.h>

void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	if (initItem && itemInfo == nullptr)
		Initalize(initItem);
}

void APickableItem::Initalize(TSubclassOf<UItemInfo> pItemInfo)
{
	itemInfo = NewObject<UItemInfo>(this, pItemInfo);
}

void APickableItem::Interact()
{
	Super::Interact();
	
	if (playerCharacter->PickupItem(itemInfo))
	{
		SetActorHiddenInGame(true);
		FindComponentByClass<UBoxComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Failed to pickup"));
}