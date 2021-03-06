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

	if (itemTypeName.Equals("Armor"))
	{
		FString itemName = (*pJsonObject)->GetStringField("itemName");
		if (itemName.Equals("BP_ItemHelmet_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemHelmet.BP_ItemHelmet_C";
			TSubclassOf<UItemHelmet> uitemSub = LoadClass<UItemHelmet>(pWorldContext, *path);
			UItemHelmet* itemInfo = NewObject<UItemHelmet>(pWorldContext,uitemSub);
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
		UItemFlashGrenade* itemInfo = NewObject<UItemFlashGrenade>(pWorldContext, uitemSub);
		if(itemInfo)
		itemInfo->SetJsonObject((*pJsonObject));

		return itemInfo;

	}
	else if (itemTypeName.Equals("Grenade"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_HandGrenade.BP_HandGrenade_C";
		TSubclassOf<UItemGrenade> uitemSub = LoadClass<UItemGrenade>(pWorldContext, *path);
		UItemGrenade* itemInfo = NewObject<UItemGrenade>(pWorldContext,uitemSub);
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
			UItem9mmMagazine* itemInfo = NewObject<UItem9mmMagazine>(pWorldContext,uitemSub);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}
		else if (itemName.Equals("BP_556Magazine_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_556Magazine.BP_556Magazine_C";
			TSubclassOf<UItem556Magazine> uitemSub = LoadClass<UItem556Magazine>(pWorldContext, *path);
			UItem556Magazine* itemInfo = NewObject<UItem556Magazine>(pWorldContext, uitemSub);
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
		UItemMedikit* itemInfo = NewObject<UItemMedikit>(pWorldContext, uitemSub);
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
			UItemM9* itemInfo = NewObject<UItemM9>(pWorldContext, uitemSub);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}
		else if (itemName.Equals("BP_WeaponM416_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM416.BP_WeaponM416_C";
			TSubclassOf<UItemM416> uitemSub = LoadClass<UItemM416>(pWorldContext, *path);
			UItemM416* itemInfo = NewObject<UItemM416>(pWorldContext, uitemSub);
			if(itemInfo)
			itemInfo->SetJsonObject((*pJsonObject));
			return itemInfo;
		}

		return nullptr;
	}
	else
		return nullptr;
}

UItemInfo* JsonSaveAndLoader::GetItemFromJsonObject(TSharedPtr<FJsonObject> pJsonObject, UWorld* pWorldContext)
{
	FString itemTypeName = pJsonObject->GetStringField("itemType");

	if (itemTypeName.Equals("Armor"))
	{
		FString itemName = pJsonObject->GetStringField("itemName");
		if (itemName.Equals("BP_ItemHelmet_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemHelmet.BP_ItemHelmet_C";
			TSubclassOf<UItemHelmet> uitemSub = LoadClass<UItemHelmet>(pWorldContext, *path);
			UItemHelmet* itemInfo = NewObject<UItemHelmet>(pWorldContext, uitemSub);
			if (itemInfo)
				itemInfo->SetJsonObject(pJsonObject);
			return itemInfo;
		}

		return nullptr;
	}
	else if (itemTypeName.Equals("FlashGrenade"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_FlashGrenade.BP_FlashGrenade_C";
		TSubclassOf<UItemFlashGrenade> uitemSub = LoadClass<UItemFlashGrenade>(pWorldContext, *path);
		UItemFlashGrenade* itemInfo = NewObject<UItemFlashGrenade>(pWorldContext, uitemSub);
		if (itemInfo)
			itemInfo->SetJsonObject(pJsonObject);

		return itemInfo;

	}
	else if (itemTypeName.Equals("Grenade"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_HandGrenade.BP_HandGrenade_C";
		TSubclassOf<UItemGrenade> uitemSub = LoadClass<UItemGrenade>(pWorldContext, *path);
		UItemGrenade* itemInfo = NewObject<UItemGrenade>(pWorldContext, uitemSub);
		if (itemInfo)
			itemInfo->SetJsonObject(pJsonObject);
		return itemInfo;
	}
	else if (itemTypeName.Equals("Magazine"))
	{
		FString itemName = pJsonObject->GetStringField("itemName");

		if (itemName.Equals("BP_9mmMagazine_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_9mmMagazine.BP_9mmMagazine_C";
			TSubclassOf<UItem9mmMagazine> uitemSub = LoadClass<UItem9mmMagazine>(pWorldContext, *path);
			UItem9mmMagazine* itemInfo = NewObject<UItem9mmMagazine>(pWorldContext, uitemSub);
			if (itemInfo)
				itemInfo->SetJsonObject(pJsonObject);
			return itemInfo;
		}
		else if (itemName.Equals("BP_556Magazine_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_556Magazine.BP_556Magazine_C";
			TSubclassOf<UItem556Magazine> uitemSub = LoadClass<UItem556Magazine>(pWorldContext, *path);
			UItem556Magazine* itemInfo = NewObject<UItem556Magazine>(pWorldContext, uitemSub);
			if (itemInfo)
				itemInfo->SetJsonObject(pJsonObject);
			return itemInfo;
		}
		return nullptr;
	}
	else if (itemTypeName.Equals("Medikit"))
	{
		const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_ItemMedikit.BP_ItemMedikit";
		TSubclassOf<UItemMedikit> uitemSub = LoadClass<UItemMedikit>(pWorldContext, *path);
		UItemMedikit* itemInfo = NewObject<UItemMedikit>(pWorldContext, uitemSub);
		if (itemInfo)
			itemInfo->SetJsonObject(pJsonObject);
		return itemInfo;
	}
	else if (itemTypeName.Equals("Weapon"))
	{
		FString itemName = pJsonObject->GetStringField("itemName");

		if (itemName.Equals("BP_WeaponM9_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM9.BP_WeaponM9_C";
			TSubclassOf<UItemM9> uitemSub = LoadClass<UItemM9>(pWorldContext, *path);
			UItemM9* itemInfo = NewObject<UItemM9>(pWorldContext, uitemSub);
			if (itemInfo)
				itemInfo->SetJsonObject(pJsonObject);
			return itemInfo;
		}
		else if (itemName.Equals("BP_WeaponM416_C"))
		{
			const FString path = "/Game/Blueprints/InvenotryAndItem/ItemInfos/BP_WeaponM416.BP_WeaponM416_C";
			TSubclassOf<UItemM416> uitemSub = LoadClass<UItemM416>(pWorldContext, *path);
			UItemM416* itemInfo = NewObject<UItemM416>(pWorldContext, uitemSub);
			if (itemInfo)
				itemInfo->SetJsonObject(pJsonObject);
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
	FString inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("itemContainers.json");

	TArray<UItemInfo*> returnArray = TArray<UItemInfo*>();
	if (FPaths::FileExists(inventoryJsonFilePath))
	{
		FString loadedContents;
		FFileHelper::LoadFileToString(loadedContents, *inventoryJsonFilePath);
		TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(loadedContents);
		TSharedPtr<FJsonObject> fullPackage;
		if (FJsonSerializer::Deserialize(reader, fullPackage))
		{
			TArray<TSharedPtr<FJsonValue>> getInventoryContents = fullPackage->GetArrayField("itemContainer");
			for (int i = 0; i < getInventoryContents.Num(); i++)
			{
				const TSharedPtr<FJsonObject>* element;
				getInventoryContents[i]->TryGetObject(element);

				//�Լ� �������
				returnArray.Add(GetItemFromJsonObject(element, pWorldContext));
			}
		}
	}

	return returnArray;
}

std::tuple<UItemWeapon*, UItemWeapon*, UItemArmor*> JsonSaveAndLoader::LoadEquipments(UWorld* pWorldContext)
{
	FString inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("equipment.json");

	TArray<UItemInfo*> returnArray = TArray<UItemInfo*>();
	if (FPaths::FileExists(inventoryJsonFilePath))
	{
		FString loadedContents;
		FFileHelper::LoadFileToString(loadedContents, *inventoryJsonFilePath);
		TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(loadedContents);
		TSharedPtr<FJsonObject> originalContents;
		FJsonSerializer::Deserialize(reader, originalContents);
		TSharedPtr<FJsonObject> primaryWeapon = originalContents->GetObjectField("primaryWeapon");
		TSharedPtr<FJsonObject> secondaryWeapon = originalContents->GetObjectField("secondaryWeapon");
		TSharedPtr<FJsonObject> helmet = originalContents->GetObjectField("helmet");

		return std::tuple<UItemWeapon*, UItemWeapon*, UItemArmor*>(Cast<UItemWeapon>(GetItemFromJsonObject(primaryWeapon, pWorldContext)), Cast<UItemWeapon>(GetItemFromJsonObject(secondaryWeapon, pWorldContext)), Cast<UItemArmor>(GetItemFromJsonObject(helmet, pWorldContext)));
	}

	return std::tuple<UItemWeapon*, UItemWeapon*, UItemArmor*>();
}

void JsonSaveAndLoader::SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave)
{
	FString inventoryJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("itemContainers.json");

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

	lastWriter->SetArrayField("itemContainer", jsonArray);
	FJsonSerializer::Serialize(lastWriter.ToSharedRef(), writer);
	FFileHelper::SaveStringToFile(outputString, *inventoryJsonFilePath);

}

void JsonSaveAndLoader::SaveEquipments(UItemWeapon* pPrimaryWeapon, UItemWeapon* pSecondaryWeapon, UItemHelmet* pHelmet)
{
	FString equipmentJsonFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetRelativePathToRoot()) + TEXT("equipment.json");

	if (pPrimaryWeapon == nullptr && pSecondaryWeapon == nullptr && pHelmet)
		return;

	FString outputString;
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);

	TSharedPtr<FJsonObject> finalResult = MakeShareable(new FJsonObject);
	if (pPrimaryWeapon)
	{
		TSharedPtr<FJsonObject> primaryWeaponJson = pPrimaryWeapon->GetJsonObject();
		finalResult->SetObjectField("primaryWeapon", primaryWeaponJson);
	}

	if (pSecondaryWeapon)
	{
		TSharedPtr<FJsonObject> secondaryWeaponJson = pSecondaryWeapon->GetJsonObject();
		finalResult->SetObjectField("secondaryWeapon", secondaryWeaponJson);
	}

	if (pHelmet)
	{
		TSharedPtr<FJsonObject> helmetJson = pHelmet->GetJsonObject();
		finalResult->SetObjectField("helmet", helmetJson);
	}

	FJsonSerializer::Serialize(finalResult.ToSharedRef(), writer);
	FFileHelper::SaveStringToFile(outputString, *equipmentJsonFilePath);
}
