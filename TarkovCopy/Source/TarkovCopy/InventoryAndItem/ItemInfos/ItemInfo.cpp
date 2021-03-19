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
FString UItemInfo::GetItemAmountString() const
{
	FString message;
	message.AppendInt(currentCapacity);
	message.AppendChar('/');
	message.AppendInt(maxCapacity);
	return message;
}

void UItemInfo::Use(AFPPlayerController* pPlayerCon)
{
	UE_LOG(LogTemp,Warning,TEXT("Item!"))
}

void UItemInfo::DropItem(AFPPlayerController* pPlayerCon)
{
	/*���⼭ �� ������ �� �����۸��� �κ��丮 �ܿ� ���� ���ۻ󿡼� ����� ������ �̷�������ϴ� ���۵�
	(��: ����������� equip �Ǿ������� �ֹ������� equip �Ǿ������� �װ� ���� �ؾ���.)�� �����Ŵ*/
	UE_LOG(LogTemp, Warning, TEXT("Drop!"))
}
