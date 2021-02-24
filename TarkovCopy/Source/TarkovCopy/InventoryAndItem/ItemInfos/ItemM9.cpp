// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "ItemM9.h"

void UItemM9::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->AddSecondary(weaponSubclass);
}
void UItemM9::DropItem(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->RemoveSecondary();
}