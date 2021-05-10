// Fill out your copyright notice in the Description page of Project Settings.


#include "Item556Magazine.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UItem556Magazine::Use(AFPPlayerController* pPlayerCon)
{

}

TSharedPtr<FJsonObject> UItem556Magazine::GetJsonObject()
{
    TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
    jsonObject->SetStringField("itemName", GetClass()->GetName());
    return jsonObject;
}
