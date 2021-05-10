// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonSaveAndLoader.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemHelmet.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemBodyArmor.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemFlashGrenade.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/Item9mmMagazine.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/Item556Magazine.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemMedikit.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemM9.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemM416.h"

TSharedPtr<FJsonObject> JsonSaveAndLoader::GetJsonObjectFromItem(UItemInfo* pItemInfo)
{

	TSharedPtr<FJsonObject> returnJsonObject;

	switch (pItemInfo->itemType)
	{
		case ItemType::ARMOR:
			UItemHelmet* itemHelmet = Cast<UItemHelmet>(pItemInfo);
			UItemBodyArmor* itemArmor = Cast<UItemBodyArmor>(pItemInfo);
			returnJsonObject = (itemHelmet)? itemHelmet->GetJsonObject():itemArmor->GetJsonObject();
			break;
		case ItemType::FLASHGRENADE:
			UItemFlashGrenade* item = Cast<UItemFlashGrenade>(pItemInfo);
			returnJsonObject = item->GetJsonObject();
			break;
		case ItemType::GRENADE:
			UItemGrenade* item = Cast<UItemFlashGrenade>(pItemInfo);
			returnJsonObject = item->GetJsonObject();
			break;
		case ItemType::MAGAZINE:
			UItem9mmMagazine* item9MM = Cast<UItem9mmMagazine>(pItemInfo);
			UItem556Magazine* item556 = Cast<UItem556Magazine>(pItemInfo);
			returnJsonObject = (item9MM) ? item9MM->GetJsonObject() : item556->GetJsonObject();
			break;
		case ItemType::MEDIKIT:
			UItemMedikit* item = Cast<UItemMedikit>(pItemInfo);
			returnJsonObject = item->GetJsonObject();
			break;
		case ItemType::WEAPON:
			UItemM9* itemM9 = Cast<UItemM9>(pItemInfo);
			UItemM416* itemM416 = Cast<UItemM416>(pItemInfo);
			returnJsonObject = (itemM9) ? itemM9->GetJsonObject() : itemM416->GetJsonObject();
			break;
		default:
			break;
	}
	return returnJsonObject;
}

UItemInfo* JsonSaveAndLoader::GetItemFromJsonObject(TSharedPtr<FJsonObject>* pJsonObject)
{
	FString itemTypeName = (*pJsonObject)->GetStringField("itemType");
	if (itemTypeName.Equals("Armor"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");

		if (itemName.Equals("UItemHelmet"))
		{
			TSoftObjectPtr<UItemHelmet> item = TSoftObjectPtr<UItemHelmet>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemHelmet.BP_ItemHelmet_C'")));
			item->SetJsonObject((*pJsonObject));
			return item.Get();
		}

		return nullptr;
	}
	else if (itemTypeName.Equals("FlashGrenade"))
	{
		TSoftObjectPtr<UItemFlashGrenade> item = TSoftObjectPtr<UItemFlashGrenade>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_FlashGrenade.BP_FlashGrenade_C'")));
		item->SetJsonObject((*pJsonObject));
		return item.Get();

	}
	else if (itemTypeName.Equals("Grenade"))
	{
		TSoftObjectPtr<UItemGrenade> item = TSoftObjectPtr<UItemGrenade>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_HandGrenade.BP_HandGrenade_C'")));
		item->SetJsonObject((*pJsonObject));
		return item.Get();
	}
	else if (itemTypeName.Equals("Magazine"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");

		if (itemName.Equals("UItem9mmMagazine"))
		{
			TSoftObjectPtr<UItem9mmMagazine> item = TSoftObjectPtr<UItem9mmMagazine>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_9mmMagazine.BP_9mmMagazine_C'")));
			item->SetJsonObject((*pJsonObject));
			return item.Get();
		}
		else if (itemName.Equals("UItem556Magazine"))
		{
			TSoftObjectPtr<UItem556Magazine> item = TSoftObjectPtr<UItem556Magazine>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_556Magazine.BP_556Magazine_C'")));
			item->SetJsonObject((*pJsonObject));
			return item.Get();
		}
		return nullptr;
	}
	else if (itemTypeName.Equals("Medikit"))
	{
		TSoftObjectPtr<UItemMedikit> item = TSoftObjectPtr<UItemMedikit>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemMedikit.BP_ItemMedikit'")));
		item->SetJsonObject((*pJsonObject));
		return item.Get();
	}
	else if (itemTypeName.Equals("Weapon"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");

		if (itemName.Equals("UItemM9"))
		{
			TSoftObjectPtr<UItemM9> item = TSoftObjectPtr<UItemM9>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM9.BP_WeaponM9_C'")));
			item->SetJsonObject((*pJsonObject));
			return item.Get();
		}
		else if (itemName.Equals("UItemM416"))
		{
			TSoftObjectPtr<UItemM416> item = TSoftObjectPtr<UItemM416>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM416.BP_WeaponM416_C'")));
			item->SetJsonObject((*pJsonObject));
			return item.Get();
		}

		return nullptr;
	}
	else
		return nullptr;
}

JsonSaveAndLoader::JsonSaveAndLoader()
{
	inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("inventory.json");

}

JsonSaveAndLoader::~JsonSaveAndLoader()
{
}

TArray<UItemInfo*> JsonSaveAndLoader::LoadBackpackItemContainers()
{
	TArray<UItemInfo*> returnArray;
	if (FPaths::FileExists(inventoryJsonFilePath))
	{
		FString loadedContents;
		FFileHelper::LoadFileToString(loadedContents, *inventoryJsonFilePath);
		TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(loadedContents);
		TSharedPtr<FJsonObject> fullPackage;
		if (FJsonSerializer::Deserialize(reader, fullPackage))
		{
			TArray<TSharedPtr<FJsonValue>> getInventoryContents = fullPackage->GetArrayField("inventory");
			for (int i = 0; i < getInventoryContents.Num(); i++)
			{
				TSharedPtr<FJsonObject>* element;
				getInventoryContents[i]->TryGetObject(element);

				//함수 만들어줘
				UItemInfo* item = GetItemFromJsonObject(element);
				returnArray.Add(item);
			}
		}
	}

	return returnArray;
}

void JsonSaveAndLoader::SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave)
{
	FString outputString;
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);

	TSharedPtr<FJsonObject> lastWriter = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	for (int i = 0; i < pItemSave.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = GetJsonObjectFromItem(pItemSave[i]);
		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObject));

		jsonArray.Add(jsonValue);
		//TODO: 이걸 스트링으로 만들어서 STRING WRITE를 해
	}

	lastWriter->SetArrayField("inventory", jsonArray);
	FJsonSerializer::Serialize(lastWriter.ToSharedRef(), writer);
	FFileHelper::SaveStringToFile(outputString, *inventoryJsonFilePath);

}
