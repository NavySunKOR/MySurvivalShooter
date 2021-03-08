// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedikit.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UItemMedikit::Use(AFPPlayerController* pPlayerCon)
{

	UE_LOG(LogTemp, Warning, TEXT("HealingItem"));
	pPlayerCon->HealPlayer(healAmount);

	Super::Use(pPlayerCon);
}