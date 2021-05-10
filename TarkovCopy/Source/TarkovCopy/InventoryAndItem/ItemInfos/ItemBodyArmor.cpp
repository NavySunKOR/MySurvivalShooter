// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBodyArmor.h"

TSharedPtr<FJsonObject> UItemBodyArmor::GetJsonObject()
{
    TSharedPtr<FJsonObject> jsonObject = Super::GetJsonObject();
    jsonObject->SetStringField("itemName", GetClass()->GetName());
    return jsonObject;
}