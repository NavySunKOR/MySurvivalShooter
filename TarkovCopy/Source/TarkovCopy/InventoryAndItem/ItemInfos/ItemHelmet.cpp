// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHelmet.h"

void UItemHelmet::Use(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->AddHelmet(this);
	Super::Use(pPlayerCon);
}
void UItemHelmet::DropItem(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->RemoveHelmet(this);
	Super::DropItem(pPlayerCon);
}

TSharedPtr<FJsonObject> UItemHelmet::GetJsonObject()
{
	TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
	jsonObject->SetStringField("itemName", GetClass()->GetName());
	return jsonObject;
}
