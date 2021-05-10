// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGrenade.h"


TSharedPtr<FJsonObject> UItemGrenade::GetJsonObject()
{
    TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
    jsonObject->SetStringField("itemName", GetClass()->GetName());
    return jsonObject;
}