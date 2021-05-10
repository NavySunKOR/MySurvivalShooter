// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFlashGrenade.h"

TSharedPtr<FJsonObject> UItemFlashGrenade::GetJsonObject()
{
    TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
    jsonObject->SetStringField("itemName", GetClass()->GetName());
    return jsonObject;
}