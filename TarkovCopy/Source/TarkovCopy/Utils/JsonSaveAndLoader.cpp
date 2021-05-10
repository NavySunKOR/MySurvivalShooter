// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "JsonSaveAndLoader.h"

TSharedPtr<FJsonObject> JsonSaveAndLoader::GetJsonObjectFromItem(UItemInfo* pItemInfo)
{
	switch (pItemInfo->itemType)
	{
		case ItemType::ARMOR :
			break;
		case ItemType::FLASHGRENADE:
			break;
		case ItemType::GRENADE:
			break;
		case ItemType::MAGAZINE:
			break;
		case ItemType::MEDIKIT:
			break;
		case ItemType::WEAPON:
			break;
		default:
			break;
	}
	return ;
}

JsonSaveAndLoader::JsonSaveAndLoader()
{
}

JsonSaveAndLoader::~JsonSaveAndLoader()
{
}

TArray<UItemInfo*> JsonSaveAndLoader::LoadBackpackItemContainers()
{
	return TArray<UItemInfo*>();
}

void JsonSaveAndLoader::SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave)
{
	for (int i = 0; i < pItemSave.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = GetJsonObjectFromItem(pItemSave[i]);
		//TODO: 이걸 스트링으로 만들어서 STRING WRITE를 해
	}
}
