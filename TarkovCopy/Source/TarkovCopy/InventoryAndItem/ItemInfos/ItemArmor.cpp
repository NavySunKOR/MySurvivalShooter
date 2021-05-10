// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemArmor.h"

FString UItemArmor::GetItemAmountString() const
{
	FString message;
	message.AppendInt(curDurability);
	message.AppendChar('/');
	message.AppendInt(maxDurability);
	return message;
}

TSharedPtr<FJsonObject> UItemArmor::GetJsonObject()
{
	TSharedPtr<FJsonObject> jsonObj = Super::GetJsonObject();
	jsonObj->SetNumberField("curDurability", curDurability);
	return jsonObj;
}

void UItemArmor::SetJsonObject(TSharedPtr<FJsonObject> pJsonObject)
{
	curDurability = pJsonObject->GetNumberField("curDurability");
	Super::SetJsonObject(pJsonObject);
}
