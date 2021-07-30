// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedikit.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UItemMedikit::Use(AFPPlayerController* pPlayerCon)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(pPlayerCon->GetPawn());
	player->HealPlayer(healAmount);
	Super::Use(pPlayerCon);
}

TSharedPtr<FJsonObject> UItemMedikit::GetJsonObject()
{
	return TSharedPtr<FJsonObject>();
}
