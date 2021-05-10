// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemM9.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"

void UItemM9::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->AddSecondary(weaponSubclass,this);
	Super::Use(pPlayerCon);
}
void UItemM9::DropItem(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->RemoveSecondary();
	Super::DropItem(pPlayerCon);
}


TSharedPtr<FJsonObject> UItemM9::GetJsonObject()
{
	TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
	jsonObject->SetStringField("itemName", GetClass()->GetName());
	return jsonObject;
}