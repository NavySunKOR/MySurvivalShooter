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


	if (pItemInfo->itemType == ItemType::ARMOR)
	{

		UItemHelmet* itemHelmet = Cast<UItemHelmet>(pItemInfo);
		UItemBodyArmor* itemArmor = Cast<UItemBodyArmor>(pItemInfo);
		returnJsonObject = (itemHelmet) ? itemHelmet->GetJsonObject() : itemArmor->GetJsonObject();
	}
	else if (pItemInfo->itemType == ItemType::FLASHGRENADE)
	{
		UItemFlashGrenade* itemFlash = Cast<UItemFlashGrenade>(pItemInfo);
		returnJsonObject = itemFlash->GetJsonObject();
	}
	else if (pItemInfo->itemType == ItemType::GRENADE)
	{
		UItemGrenade* itemGrenade = Cast<UItemGrenade>(pItemInfo);
		returnJsonObject = itemGrenade->GetJsonObject();
	}
	else if (pItemInfo->itemType == ItemType::MAGAZINE)
	{
		UItem9mmMagazine* item9MM = Cast<UItem9mmMagazine>(pItemInfo);
		UItem556Magazine* item556 = Cast<UItem556Magazine>(pItemInfo);
		returnJsonObject = (item9MM) ? item9MM->GetJsonObject() : item556->GetJsonObject();
	}
	else if (pItemInfo->itemType == ItemType::MEDIKIT)
	{
		UItemMedikit* itemMedikit = Cast<UItemMedikit>(pItemInfo);
		returnJsonObject = itemMedikit->GetJsonObject();
	}
	else if (pItemInfo->itemType == ItemType::WEAPON)
	{
		UItemM9* itemM9 = Cast<UItemM9>(pItemInfo);
		UItemM416* itemM416 = Cast<UItemM416>(pItemInfo);
		returnJsonObject = (itemM9) ? itemM9->GetJsonObject() : itemM416->GetJsonObject();
	}

	return returnJsonObject;
}

UItemInfo* JsonSaveAndLoader::GetItemFromJsonObject(const TSharedPtr<FJsonObject>* pJsonObject, UWorld* pWorldContext)
{
	FString itemTypeName = (*pJsonObject)->GetStringField("itemType");
	UE_LOG(LogTemp,Warning,TEXT("HAHO : %s"), *itemTypeName)

	if (itemTypeName.Equals("Armor"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");
		UE_LOG(LogTemp, Warning, TEXT("HAHO name : %s"), *itemName)
		if (itemName.Equals("BP_ItemHelmet_C"))
		{

			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemHelmet.BP_ItemHelmet_C";
			TSubclassOf<UItemHelmet> uitemSub = LoadClass<UItemHelmet>(pWorldContext, *path);
			UItemInfo* itemInfo = NewObject<UItemHelmet>(uitemSub);
			UE_LOG(LogTemp, Warning, TEXT("UItemHelmet : %d"), itemInfo);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}

		return nullptr;
	}
	else if (itemTypeName.Equals("FlashGrenade"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_FlashGrenade.BP_FlashGrenade_C";
		TSubclassOf<UItemFlashGrenade> uitemSub = LoadClass<UItemFlashGrenade>(pWorldContext, *path);
		UItemInfo* itemInfo = NewObject<UItemFlashGrenade>(uitemSub);
		UE_LOG(LogTemp, Warning, TEXT("UItemFlashGrenade : %d"), itemInfo);
		if(itemInfo)
		itemInfo->SetJsonObject((*pJsonObject));

		return itemInfo;

	}
	else if (itemTypeName.Equals("Grenade"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_HandGrenade.BP_HandGrenade_C";
		TSubclassOf<UItemGrenade> uitemSub = LoadClass<UItemGrenade>(pWorldContext, *path);
		UItemInfo* itemInfo = NewObject<UItemGrenade>(uitemSub);
		UE_LOG(LogTemp, Warning, TEXT("UItemGrenade : %d"), itemInfo);
		if(itemInfo)
		itemInfo->SetJsonObject((*pJsonObject));
		return itemInfo;
	}
	else if (itemTypeName.Equals("Magazine"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");

		if (itemName.Equals("BP_9mmMagazine_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_9mmMagazine.BP_9mmMagazine_C";
			TSubclassOf<UItem9mmMagazine> uitemSub = LoadClass<UItem9mmMagazine>(pWorldContext, *path);
			UItemInfo* itemInfo = NewObject<UItem9mmMagazine>(uitemSub);
			UE_LOG(LogTemp, Warning, TEXT("UItem9mmMagazine : %d"), itemInfo);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}
		else if (itemName.Equals("BP_556Magazine_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_556Magazine.BP_556Magazine_C";
			TSubclassOf<UItem556Magazine> uitemSub = LoadClass<UItem556Magazine>(pWorldContext, *path);
			UItemInfo* itemInfo = NewObject<UItem556Magazine>(uitemSub);
			UE_LOG(LogTemp, Warning, TEXT("UItem556Magazine : %d"), itemInfo);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}
		return nullptr;
	}
	else if (itemTypeName.Equals("Medikit"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemMedikit.BP_ItemMedikit";
		TSubclassOf<UItemMedikit> uitemSub = LoadClass<UItemMedikit>(pWorldContext, *path);
		UItemInfo* itemInfo = NewObject<UItemMedikit>(uitemSub);
		UE_LOG(LogTemp, Warning, TEXT("UItemMedikit : %d"), itemInfo);
		if(itemInfo)
		itemInfo->SetJsonObject((*pJsonObject));
		return itemInfo;
	}
	else if (itemTypeName.Equals("Weapon"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");

		if (itemName.Equals("BP_WeaponM9_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM9.BP_WeaponM9_C";
			TSubclassOf<UItemM9> uitemSub = LoadClass<UItemM9>(pWorldContext, *path);
			UItemInfo* itemInfo = NewObject<UItemM9>(uitemSub);
			UE_LOG(LogTemp, Warning, TEXT("UItemM9 : %d"), itemInfo);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}
		else if (itemName.Equals("BP_WeaponM416_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM416.BP_WeaponM416_C";
			TSubclassOf<UItemM416> uitemSub = LoadClass<UItemM416>(pWorldContext, *path);
			UItemInfo* itemInfo = NewObject<UItemM416>(uitemSub);
			UE_LOG(LogTemp, Warning, TEXT("UItemM416 : %d"), itemInfo);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}

		return nullptr;
	}
	else
		return nullptr;
}

JsonSaveAndLoader::JsonSaveAndLoader()
{
}

JsonSaveAndLoader::~JsonSaveAndLoader()
{
}

TArray<UItemInfo*> JsonSaveAndLoader::LoadBackpackItemContainers(UWorld* pWorldContext)
{
	FString inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("inventory.json");

	TArray<UItemInfo*> returnArray = TArray<UItemInfo*>();
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
				const TSharedPtr<FJsonObject>* element;
				getInventoryContents[i]->TryGetObject(element);

				//함수 만들어줘
				UItemInfo* item = GetItemFromJsonObject(element, pWorldContext);
				returnArray.Add(item);
			}
		}
	}

	return returnArray;
}

void JsonSaveAndLoader::SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave)
{
	FString inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("inventory.json");

	FString outputString;
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);

	TSharedPtr<FJsonObject> lastWriter = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> jsonArray;
	for (int i = 0; i < pItemSave.Num(); i++)
	{
		TSharedPtr<FJsonObject> jsonObject = GetJsonObjectFromItem(pItemSave[i]);
		TSharedRef<FJsonValueObject> jsonValue = MakeShareable(new FJsonValueObject(jsonObject));

		jsonArray.Add(jsonValue);
	}

	lastWriter->SetArrayField("inventory", jsonArray);
	FJsonSerializer::Serialize(lastWriter.ToSharedRef(), writer);
	FFileHelper::SaveStringToFile(outputString, *inventoryJsonFilePath);

}
