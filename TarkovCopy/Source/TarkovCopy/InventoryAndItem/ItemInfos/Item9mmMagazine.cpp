// Fill out your copyright notice in the Description page of Project Settings.

#include "Item9mmMagazine.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"


void UItem9mmMagazine::Use(AFPPlayerController* pPlayerCon)
{

}

TSharedPtr<FJsonObject> UItem9mmMagazine::GetJsonObject()
{
    TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
    jsonObject->SetStringField("itemName", GetClass()->GetName());
    return jsonObject;
}
