// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemM416.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"

void UItemM416::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->AddPrimary(weaponSubclass, this);
	Super::Use(pPlayerCon);
}
void UItemM416::DropItem(AFPPlayerController* pPlayerCon)
{
	//DropFromPrimary
	pPlayerCon->RemovePrimary();
	Super::DropItem(pPlayerCon);

}

TSharedPtr<FJsonObject> UItemM416::GetJsonObject()
{
	TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
	jsonObject->SetStringField("itemName", GetClass()->GetName());
	return jsonObject;
}