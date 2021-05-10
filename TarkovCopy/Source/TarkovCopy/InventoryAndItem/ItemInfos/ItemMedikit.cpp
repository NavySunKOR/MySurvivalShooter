// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedikit.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UItemMedikit::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->HealPlayer(healAmount);
	Super::Use(pPlayerCon);
}

TSharedPtr<FJsonObject> UItemMedikit::GetJsonObject()
{
	return TSharedPtr<FJsonObject>();
}
