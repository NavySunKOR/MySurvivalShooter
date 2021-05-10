// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UItemInfo;
static class TARKOVCOPY_API JsonSaveAndLoader
{

private: 
	static TSharedPtr<FJsonObject> GetJsonObjectFromItem(UItemInfo* pItemInfo);
	static UItemInfo* GetItemFromJsonObject(TSharedPtr<FJsonObject>* pJsonObject);
	static FString inventoryJsonFilePath;

public:
	JsonSaveAndLoader();
	~JsonSaveAndLoader();
	static TArray<UItemInfo*> LoadBackpackItemContainers();
	static void SaveBackpackItemContainers(TArray<UItemInfo*> pItemSave);

};
