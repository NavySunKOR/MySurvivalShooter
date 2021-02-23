// Fill out your copyright notice in the Description page of Project Settings.


#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/Interactable/PickableItem.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "ItemInfo.h"

//UItemInfo::UItemInfo()
//{
//	rect = FSlateRect();
//	rect.Left = 0;
//	rect.Top = 0;
//	rect.Right = 0;
//	rect.Bottom = 0;
//}
//
//UItemInfo::UItemInfo(float leftPos, float topPos, float width, float height)
//{
//	rect = FSlateRect(leftPos, topPos, leftPos + width, topPos - height);
//}
//
//UItemInfo::~UItemInfo()
//{
//	delete spriteToUse;
//	delete meshToDrop;
//}
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
UItemInfo UItemInfo::operator=(UItemInfo& other)
{
	UItemInfo items;
	items.width = other.width;
	items.height = other.height;
	items.spriteToUse = other.spriteToUse;
	items.meshToDrop = other.meshToDrop;
	items.maxCapacity = other.maxCapacity;
	items.currentCapacity = other.currentCapacity;

	return items;
}
bool UItemInfo::Use()
{
	return false;
}

void UItemInfo::DropItem(AFPPlayerController* pPlayerCon)
{
	/*���⼭ �� ������ �� �����۸��� �κ��丮 �ܿ� ���� ���ۻ󿡼� ����� ������ �̷�������ϴ� ���۵�
	(��: ����������� equip �Ǿ������� �ֹ������� equip �Ǿ������� �װ� ���� �ؾ���.)�� �����Ŵ*/
}
