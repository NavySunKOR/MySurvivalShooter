// Fill out your copyright notice in the Description page of Project Settings.



#include "ItemIcon.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>
#include <Components/OverlaySlot.h>
#include <Blueprint/WidgetTree.h>
#include "ItemInfo.h"
#include "ItemWeapon.h"
#include "TarkovCopy/PublicProperty/UMGPublicProperites.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UItemIcon::AdjustPositionRectForItemContainer(FSlateRect& positionIntRect, FSlateRect itemContainerIntRect)
{
	//���콺 Ŀ���� �������� �ƴ϶� ������ ���̰� �� ��� ��츦 ���
	if (positionIntRect.Left < itemContainerIntRect.Left || positionIntRect.Top < itemContainerIntRect.Left)
	{
		if (positionIntRect.Left < itemContainerIntRect.Left)
		{
			positionIntRect.Left = (int)itemContainerIntRect.Left;
			positionIntRect.Right = (int)(positionIntRect.Left + itemInfo->width);
		}
		if (positionIntRect.Top < itemContainerIntRect.Top)
		{
			positionIntRect.Top = (int)itemContainerIntRect.Top;
			positionIntRect.Bottom = (int)(positionIntRect.Top + itemInfo->height);
		}
	}

	//���콺 Ŀ���� �������� �ƴ϶� ������ ���̰� �� ��� ��츦 ���
	if (positionIntRect.Right > itemContainerIntRect.Right || positionIntRect.Bottom > itemContainerIntRect.Bottom)
	{
		if (positionIntRect.Right > itemContainerIntRect.Right)
		{
			positionIntRect.Right = (int)itemContainerIntRect.Right;
			positionIntRect.Left = (int)((int)positionIntRect.Right - itemInfo->width);
		}
		if (positionIntRect.Bottom > itemContainerIntRect.Bottom)
		{
			positionIntRect.Bottom = (int)itemContainerIntRect.Bottom;
			positionIntRect.Top = (int)((int)positionIntRect.Bottom - itemInfo->height);
		}
	}
}



void UItemIcon::Init(UItemInfo* pItemInfo, UInventory* pInven, AFPPlayerController* pController)
{
	itemInfo = pItemInfo;
	invenRef = pInven;
	controllerRef = pController;

	//���� ������Ʈ �ʱ�ȭ
	iconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	iconImage->SetBrushFromTexture(pItemInfo->spriteToUse);
	WidgetTree->RootWidget = GetWidgetFromName(TEXT("Parent"));

	canvasSlot = Cast<UCanvasPanelSlot>(Slot);

	if (canvasSlot != nullptr)
	{
		canvasSlot->SetSize(FVector2D(itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
		canvasSlot->SetPosition(FVector2D(itemInfo->left * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->top * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
	
		UOverlaySlot* imageOverlay = Cast<UOverlaySlot>(iconImage->Slot);
		if (imageOverlay)
		{
			imageOverlay->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			imageOverlay->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}

	//�巡�� �� ��� ������Ʈ
	dragObject = CreateWidget<UUserWidget>(this, origin);
	dragObjectImage = Cast<UImage>(dragObject->GetWidgetFromName(TEXT("ICON")));
	dragObjectImage->SetBrushFromTexture(itemInfo->spriteToUse);
	dragObjectImage->SetOpacity(0.3f);
	dragObjectSlot = Cast<UCanvasPanelSlot>(dragObjectImage->Slot);
	if (dragObjectSlot != nullptr)
	{
		dragObjectSlot->SetSize(FVector2D(itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
	}
}

void UItemIcon::OpenDetailPanel()
{
	controllerRef->OpenItemDetailPanel(this);
}

void UItemIcon::UseItem()
{
	if (itemInfo->isRightClickUsable && invenRef->HasItem(itemInfo))
	{
		itemInfo->Use(controllerRef); // ���⼭ consumable���� �ƴ��� ��������.
		bool isEmpty = invenRef->UseItem(itemInfo);

		if (isEmpty)
		{
			itemInfo->refInventory = nullptr;
			invenRef->RemoveItem(itemInfo);
			controllerRef->DropItem(this);
		}
	}
}

void UItemIcon::DropItem()
{
	bool isEmpty = invenRef->DropItem(itemInfo);
	if (isEmpty)
	{
		itemInfo->DropItem(controllerRef);
		itemInfo->refInventory = nullptr;
		controllerRef->DropItem(this);
	}
}

FString UItemIcon::GetAmountText() const
{
	return itemInfo->GetItemAmountString();
}

FReply UItemIcon::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	FEventReply reply;
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		OpenDetailPanel();
	}

	return reply.NativeReply;
}


void UItemIcon::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	dragDrop = nullptr;
	dragDrop = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	dragDrop->Payload = this;
	dragDrop->DefaultDragVisual = dragObject;
	dragDrop->Pivot = EDragPivot::MouseDown;
	OutOperation = dragDrop;

	handledDragDrop = InMouseEvent;
	isDragged = true;
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

}



void UItemIcon::OnDropAction(FVector2D lastMousePosition)
{
	//���콺 ��ġ�� ���� ������ RECT ���� - ��� ��쿡�� ���̴ϱ� �̸� ������.
	FSlateRect positionIntRect;
	positionIntRect.Left = (int)((lastMousePosition.X / UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH) - (itemInfo->width / 2));
	positionIntRect.Top = (int)((lastMousePosition.Y / UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) - (itemInfo->height / 2));
	positionIntRect.Right = (int)(positionIntRect.Left + itemInfo->width);
	positionIntRect.Bottom = (int)(positionIntRect.Top + itemInfo->height);

	//���콺�� �κ��丮 �ȿ� �ִٸ�
	if (controllerRef->itemContainerRect.ContainsPoint(lastMousePosition))
	{
		//�κ��丮�� int rect�� ���ġ
		FSlateRect itemContainerIntRect = controllerRef->itemContainerRect;
		itemContainerIntRect.Left = (int)(itemContainerIntRect.Left/UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
		itemContainerIntRect.Right = (int)(itemContainerIntRect.Right/UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
		itemContainerIntRect.Top = (int)(itemContainerIntRect.Top/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);
		itemContainerIntRect.Bottom = (int)(itemContainerIntRect.Bottom/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);

		AdjustPositionRectForItemContainer(positionIntRect, itemContainerIntRect);

		//�� �������� ���ݿ� �ִ��ǰ� �ƴϸ� �κ� ������ �ű�� ���ΰ�?
		if (controllerRef->IsInItemContainer(this))
		{
			//���� ������ ��ġ�� �ϴ� ���ش�(��ĭ�� �ű�°� �����ϰԲ�)
			controllerRef->StartMoveItemPos(itemInfo);
			//�ش���ġ�� �ű� �� �ִٸ� �ű�� �ƴϸ� ���� ��ġ�� ������.
			if (controllerRef->CanItemMoveTo(positionIntRect))
			{
				controllerRef->MoveItemTo(itemInfo, positionIntRect);
			}
			else
			{
				controllerRef->FailedToMoveItemPos(itemInfo);
			}
		}
		else if (controllerRef->IsInEquipmentSlot(this))
		{
			if (controllerRef->CanItemMoveTo(positionIntRect))
			{
				controllerRef->MoveItemTo(itemInfo, positionIntRect);
				if (itemInfo->itemType == ItemType::WEAPON)
				{
					UItemWeapon* itemWeapon = (UItemWeapon*)itemInfo;
					if (itemWeapon->weaponType == WeaponType::PRIMARY)
					{
						controllerRef->RemovePrimary();
					}
					else
					{
						controllerRef->RemoveSecondary();
					}
				}
				else if (itemInfo->itemType == ItemType::ARMOR)
				{
					UItemHelmet* itemHelmet = (UItemHelmet*)itemInfo;
					if (itemHelmet != nullptr)
					{
						controllerRef->RemoveHelmet(itemHelmet);
					}
				}

			}
		}
		else
		{
			//TODO:�̰� ���״� �� ������ ���ؼ� ���� �ľ� �� �ذ��Ұ�(�Ƹ� �������� ���ȴµ� ui���� remove�� �ȵǴ� ����̰ų� ó���� �ȵ� ��� �� ���̴�)
		}
	}
	else if (controllerRef->primaryWeaponContainerRect.ContainsPoint(lastMousePosition))
	{
		UItemWeapon* weapon = Cast<UItemWeapon>(itemInfo);
		if (weapon != nullptr && weapon->weaponType == WeaponType::PRIMARY)
		{
			UseItem();
		}
	}
	else if (controllerRef->secondaryWeaponContainerRect.ContainsPoint(lastMousePosition))
	{
		UItemWeapon* weapon = Cast<UItemWeapon>(itemInfo);
		if (weapon != nullptr && weapon->weaponType == WeaponType::SECONDARY)
		{
			UseItem();
		}
	}
	else if (controllerRef->helmetContainerRect.ContainsPoint(lastMousePosition))
	{
		UItemHelmet* helmet = Cast<UItemHelmet>(itemInfo);
		if (helmet != nullptr)
		{
			UseItem();
		}
	}
	else //���콺�� �κ��丮 ���� �ۿ� ������
	{
		DropItem();
	}
	isDragged = false;
}


void UItemIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isDragged && !handledDragDrop.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FVector2D mousePos;
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);
		controllerRef->GetMousePosition(mousePos.X, mousePos.Y);
		mousePos /= scale;

		OnDropAction(mousePos);
	}

}