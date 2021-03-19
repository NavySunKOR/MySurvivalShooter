// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeapon.h"

void UItemWeapon::Use(AFPPlayerController* pPlayerCon)
{
	Super::Use(pPlayerCon);
}

void UItemWeapon::DropItem(AFPPlayerController* pPlayerCon)
{
	Super::DropItem(pPlayerCon);
}

FString  UItemWeapon::GetItemAmountString() const
{
	FString message;
	message.AppendInt(currentMagazineAmmo);
	message.AppendChar('/');
	message.AppendInt(maxMagazineAmmo);
	return message;
}
