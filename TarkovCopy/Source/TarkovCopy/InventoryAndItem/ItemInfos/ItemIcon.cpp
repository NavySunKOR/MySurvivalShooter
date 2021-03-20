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
	//마우스 커서만 빗나간게 아니라 아이템 길이가 길어서 벗어난 경우를 대비
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

	//마우스 커서만 빗나간게 아니라 아이템 길이가 길어서 벗어난 경우를 대비
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

	//내부 컴포넌트 초기화
	iconImage = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	iconImage->SetBrushFromTexture(pItemInfo->spriteToUse);
	WidgetTree->RootWidget = GetWidgetFromName(TEXT("Parent"));

	UCanvasPanelSlot* canvas = Cast<UCanvasPanelSlot>(Slot);

	if (canvas != nullptr)
	{
		canvas->SetSize(FVector2D(itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
		canvas->SetPosition(FVector2D(itemInfo->left * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->top * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
	
		UOverlaySlot* imageOverlay = Cast<UOverlaySlot>(iconImage->Slot);
		if (imageOverlay)
		{
			imageOverlay->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			imageOverlay->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}

	//드래그 앤 드롭 오브젝트
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
	UE_LOG(LogTemp,Warning,TEXT("Using items"))
	if (invenRef->HasItem(itemInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Use "))
		itemInfo->Use(controllerRef); // 여기서 consumable인지 아닌지 결정해줌.

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnReleased"))
	}
	return reply.NativeReply;
}


void UItemIcon::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDragDetected"))
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
	UE_LOG(LogTemp, Warning, TEXT("OnDropAction"))

	//마우스 위치로 부터 아이템 RECT 생성 - 어느 경우에도 쓰이니까 미리 생성함.
	FSlateRect positionIntRect;
	positionIntRect.Left = (int)((lastMousePosition.X / UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH) - (itemInfo->width / 2));
	positionIntRect.Top = (int)((lastMousePosition.Y / UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) - (itemInfo->height / 2));
	positionIntRect.Right = (int)(positionIntRect.Left + itemInfo->width);
	positionIntRect.Bottom = (int)(positionIntRect.Top + itemInfo->height);


	//마우스가 인벤토리 안에 있다면
	if (controllerRef->itemContainerRect.ContainsPoint(lastMousePosition))
	{
		//인벤토리를 int rect로 재배치
		FSlateRect itemContainerIntRect = controllerRef->itemContainerRect;
		itemContainerIntRect.Left = (int)(itemContainerIntRect.Left/UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
		itemContainerIntRect.Right = (int)(itemContainerIntRect.Right/UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
		itemContainerIntRect.Top = (int)(itemContainerIntRect.Top/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);
		itemContainerIntRect.Bottom = (int)(itemContainerIntRect.Bottom/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);

		AdjustPositionRectForItemContainer(positionIntRect, itemContainerIntRect);

		//현재 아이템 위치를 일단 없앤다(한칸씩 옮기는거 가능하게끔)
		//TODO: 주무기 또는 보조무기로 장착한 아이템이 인벤토리로 다시 옮기는 경우를 예외처리 할것
		controllerRef->StartMoveItemPos(itemInfo);
		//해당위치로 옮길 수 있다면 옮기고 아니면 원래 위치로 돌린다.
		if (controllerRef->CanItemMoveTo(positionIntRect))
		{
			UE_LOG(LogTemp, Warning, TEXT("Move item"))
			controllerRef->MoveItemTo(itemInfo, positionIntRect);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Move item"))
			controllerRef->FailedToMoveItemPos(itemInfo);
		}
	}
	else if (controllerRef->primaryWeaponContainerRect.ContainsPoint(lastMousePosition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Primary"))
		UItemWeapon* weapon = Cast<UItemWeapon>(itemInfo);
		if (weapon != nullptr && weapon->weaponType == WeaponType::PRIMARY)
		{
			UE_LOG(LogTemp, Warning, TEXT("PrimaryCompleteo"))
			//TODO: 주무기로 장착하는 과정 진행하기
			//TODO: 여기서 AddWeapon할 때 WeaponRect로 설정
			UseItem();
		}
	}
	else if (controllerRef->secondaryWeaponContainerRect.ContainsPoint(lastMousePosition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Secondary"))
		UItemWeapon* weapon = Cast<UItemWeapon>(itemInfo);
		if (weapon != nullptr && weapon->weaponType == WeaponType::SECONDARY)
		{
			UE_LOG(LogTemp, Warning, TEXT("SeconCompleteo"))
			//TODO: 보조무기로 장착하는 과정 진행하기
			//TODO: 여기서 AddWeapon할 때 WeaponRect로 설정
			UseItem();
		}
	}
	else //마우스가 인벤토리 영역 밖에 있으면
	{
		DropItem();
		UE_LOG(LogTemp, Warning, TEXT("DropItem"))
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