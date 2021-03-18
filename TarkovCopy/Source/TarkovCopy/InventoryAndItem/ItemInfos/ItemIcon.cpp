// Fill out your copyright notice in the Description page of Project Settings.



#include "ItemIcon.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/VerticalBox.h>
#include <Blueprint/WidgetTree.h>
#include "ItemInfo.h"
#include "TarkovCopy/PublicProperty/UMGPublicProperites.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UItemIcon::Init(UItemInfo* pItemInfo, UInventory* pInven, AFPPlayerController* pController)
{
	itemInfo = pItemInfo;
	invenRef = pInven;
	controllerRef = pController;
	iconImage = WidgetTree->ConstructWidget<UImage>();
	iconImage->SetBrushFromTexture(pItemInfo->spriteToUse);
	WidgetTree->RootWidget = iconImage;

	UCanvasPanelSlot* canvas = Cast<UCanvasPanelSlot>(Slot);
	if(canvas != nullptr)
	{
		canvas->SetSize(FVector2D(itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
		canvas->SetPosition(FVector2D(itemInfo->left * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->top * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
	}


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
	UE_LOG(LogTemp, Warning, TEXT("UsingItem"));
	if (invenRef->HasItem(itemInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("We gonna use %s"), *itemInfo->GetClass()->GetName());
		itemInfo->Use(controllerRef); // 여기서 consumable인지 아닌지 결정해줌.

		bool isEmpty = invenRef->UseItem(itemInfo);

		if (isEmpty)
		{
			itemInfo->refInventory = nullptr;
			invenRef->RemoveItem(itemInfo);
			RemoveFromParent();
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
		RemoveFromParent();
	}
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

		/*FVector2D mousePos;
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);
		controllerRef->GetMousePosition(mousePos.X, mousePos.Y);
		mousePos /= scale;*/
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

		//좌표계산해서 드랍인지 아니면 스위칭인지 판단할것.
		
		FSlateRect rect;
		rect.Left = (int)controllerRef->inventoryContainerSlot->GetPosition().X;
		rect.Top = (int)controllerRef->inventoryContainerSlot->GetPosition().Y;
		rect.Right = (int)rect.Left + controllerRef->inventoryContainerSlot->GetSize().X; // 스타트 지점이 0 인 경우 
		rect.Bottom = (int)rect.Top + controllerRef->inventoryContainerSlot->GetSize().Y;
		if (rect.ContainsPoint(lastMousePosition))
		{
			FSlateRect positionIntRect;
			positionIntRect.Left = (int)((lastMousePosition.X / UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH) - (itemInfo->width / 2));
			positionIntRect.Top = (int)((lastMousePosition.Y / UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT)  - (itemInfo->height/2));
			positionIntRect.Right = (int)(positionIntRect.Left + itemInfo->width);
			positionIntRect.Bottom = (int)(positionIntRect.Top + itemInfo->height);



			rect.Left = (int)(rect.Left /UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
			rect.Right = (int)(rect.Right/UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH);
			rect.Top = (int)(rect.Top/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);
			rect.Bottom = (int)(rect.Bottom/UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT);

			UE_LOG(LogTemp, Warning, TEXT("item edge right : %f bottom : %f , container right : %f  bottom : %f"), positionIntRect.Right, positionIntRect.Bottom, rect.Right, rect.Bottom);



			//마우스 커서만 빗나간게 아니라 아이템 길이가 길어서 벗어난 경우를 대비
			if (positionIntRect.Left < rect.Left  || positionIntRect.Top < rect.Left)
			{
				if (positionIntRect.Left < rect.Left)
				{
					positionIntRect.Left = (int)rect.Left;
					positionIntRect.Right = (int)(positionIntRect.Left + itemInfo->width);
				}
				if (positionIntRect.Top < rect.Top)
				{
					positionIntRect.Top = (int)rect.Top;
					positionIntRect.Bottom = (int)(positionIntRect.Top + itemInfo->height);
				}
			}


			if (positionIntRect.Right > rect.Right || positionIntRect.Bottom > rect.Bottom)
			{
				if (positionIntRect.Right > rect.Right)
				{
					positionIntRect.Right = (int)rect.Right;
					positionIntRect.Left = (int)((int)positionIntRect.Right - itemInfo->width);
				}
				if (positionIntRect.Bottom > rect.Bottom)
				{
					positionIntRect.Bottom = (int)rect.Bottom;
					positionIntRect.Top = (int)((int)positionIntRect.Bottom - itemInfo->height);
				}
			}


			UE_LOG(LogTemp, Warning, TEXT("Mouse position : %s"), *positionIntRect.ToString())

			//현재 아이템 위치를 일단 없앤다(한칸씩 옮기는거 가능하게끔)
			controllerRef->StartMoveItemPos(itemInfo);

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
		else
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