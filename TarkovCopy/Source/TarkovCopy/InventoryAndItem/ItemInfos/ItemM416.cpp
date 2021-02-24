// Fill out your copyright notice in the Description page of Project Settings.


#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "ItemM416.h"

void UItemM416::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->AddPrimary(weaponSubclass);
}
void UItemM416::DropItem(AFPPlayerController* pPlayerCon)
{
	//DropFromPrimary
	pPlayerCon->RemovePrimary();

}