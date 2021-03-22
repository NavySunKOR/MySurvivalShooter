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
