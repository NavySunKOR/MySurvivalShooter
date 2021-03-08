// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemM416.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"

void UItemM416::Use(AFPPlayerController* pPlayerCon)
{
	UE_LOG(LogTemp, Warning, TEXT("EquipPrimaryWeapon"))
	pPlayerCon->AddPrimary(weaponSubclass);
	Super::Use(pPlayerCon);
}
void UItemM416::DropItem(AFPPlayerController* pPlayerCon)
{
	//DropFromPrimary
	UE_LOG(LogTemp, Warning, TEXT("DropPrimaryWeapon"))
	pPlayerCon->RemovePrimary();
	Super::DropItem(pPlayerCon);

}