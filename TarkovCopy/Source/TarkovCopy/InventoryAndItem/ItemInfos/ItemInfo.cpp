// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

void UItemInfo::InitRect(float pLeft, float pTop)
{
	rect = FSlateRect();
	left = pLeft;
	top = pTop;
	rect.Left = left;
	rect.Top = top;
	rect.Right = left + width;
	rect.Bottom = top - height;
}


FString UItemInfo::GetItemTypeEnumString() const
{
	FString enumString;

	switch (itemType)
	{
		case ItemType::ARMOR: 
			enumString.Append("Armor");
			break;
		case ItemType::FLASHGRENADE:
			enumString.Append("FlashGrenade"); 
			break;
		case ItemType::GRENADE: 
			enumString.Append("Grenade");
			break;
		case ItemType::MAGAZINE:
			enumString.Append("Magazine"); 
			break;
		case ItemType::MEDIKIT:
			enumString.Append("Medikit"); 
			break;
		case ItemType::WEAPON:
			enumString.Append("Weapon");
			break;
		default: break;
	}

	return enumString;
}

void UItemInfo::SetItemTypeEnumString(FString pString)
{
	//FString�� switch�� �ȵǼ� �̰ɷ� ��.
	if (pString.Equals("Armor"))
		itemType = ItemType::ARMOR;
	else if (pString.Equals("FlashGrenade"))
		itemType = ItemType::FLASHGRENADE;
	else if (pString.Equals("Grenade"))
		itemType = ItemType::GRENADE;
	else if (pString.Equals("Magazine"))
		itemType = ItemType::MAGAZINE;
	else if (pString.Equals("Medikit"))
		itemType = ItemType::MEDIKIT;
	else if (pString.Equals("Weapon"))
		itemType = ItemType::WEAPON;
	else
		itemType = ItemType::UNDEFINED;

}

FString UItemInfo::GetItemAmountString() const
{
	FString message;
	message.AppendInt(currentCapacity);
	message.AppendChar('/');
	message.AppendInt(maxCapacity);
	return message;
}

TSharedPtr<FJsonObject> UItemInfo::GetJsonObject()
{
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);

	//���� : �����ۿ� ����Ǵ� �ּ����� ������ �ִ´� - �ε忡 �����ϸ� �Ʒ� ������ �ٲ��ش�.
	//jsonObject�� ���� �������� ������. 
	jsonObject->SetStringField("itemType", GetItemTypeEnumString());
	jsonObject->SetNumberField("width", width);
	jsonObject->SetNumberField("height", height);
	jsonObject->SetNumberField("left", rect.Left);
	jsonObject->SetNumberField("top", rect.Top);
	jsonObject->SetNumberField("currentCapacity", currentCapacity);
	return jsonObject;
}

void UItemInfo::SetJsonObject(TSharedPtr<FJsonObject> pJsonObject)
{
	SetItemTypeEnumString(pJsonObject->GetStringField("itemType"));
	width = pJsonObject->GetNumberField("width");
	height = pJsonObject->GetNumberField("height");
	rect.Left = pJsonObject->GetNumberField("left");
	rect.Top = pJsonObject->GetNumberField("top");
	currentCapacity = pJsonObject->GetNumberField("currentCapacity");
}

void UItemInfo::Use(AFPPlayerController* pPlayerCon)
{

}

void UItemInfo::DropItem(AFPPlayerController* pPlayerCon)
{
	/*���⼭ �� ������ �� �����۸��� �κ��丮 �ܿ� ���� ���ۻ󿡼� ����� ������ �̷�������ϴ� ���۵�
	(��: ����������� equip �Ǿ������� �ֹ������� equip �Ǿ������� �װ� ���� �ؾ���.)�� �����Ŵ*/
}
