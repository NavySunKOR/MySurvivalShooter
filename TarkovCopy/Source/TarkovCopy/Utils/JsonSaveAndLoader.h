// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UItemInfo;
class UItemHelmet;
class UItemWeapon;

static class TARKOVCOPY_API JsonSaveAndLoader
{

private: 
	static TSharedPtr<FJsonObject> GetJsonObjectFromItem(UItemInfo* pItemInfo);
	static UItemInfo* GetItemFromJsonObject(const TSharedPtr<FJsonObject>* pJsonObject, UWorld* pWorldContext);

public:
	JsonSaveAndLoader();
	~JsonSaveAndLoader();
	static TArray<UItemInfo*> LoadBackpackItemContainers(UWorld* pWorldContext);
	static std::tuple<UItemWeapon*, UItemWeapon*, UItemHelmet*> LoadEquipments();
	static void SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave);
	static void SaveEquipments(UItemWeapon* pPrimaryWeapon, UItemWeapon* pSecondaryWeapon,UItemHelmet* pHelmet);

};
