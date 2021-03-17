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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting size failllllled!"));
	}

	dragObject = CreateWidget<UUserWidget>(this, origin);
	dragObjectImage = Cast<UImage>(dragObject->GetWidgetFromName(TEXT("ICON")));
	dragObjectImage->SetBrushFromTexture(itemInfo->spriteToUse);
	dragObjectImage->SetOpacity(0.3f);
	//dragObject->AddToViewport(6);
	dragObjectSlot = Cast<UCanvasPanelSlot>(dragObjectImage->Slot);
	UE_LOG(LogTemp, Warning, TEXT("original class of dragObjectSlot : %d or %d"), dragObjectSlot, dragObjectImage->Slot)

	if (dragObjectSlot != nullptr)
	{
		dragObjectSlot->SetSize(FVector2D(itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));

		UE_LOG(LogTemp, Warning, TEXT("original size of dragObjectSlot : %s"), *dragObjectSlot->GetSize().ToString())
	}
	dragObject->SetVisibility(ESlateVisibility::Hidden);

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
	return reply.NativeReply;
}

void UItemIcon::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("OnDragEnter"))
	

}

void UItemIcon::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp,Warning,TEXT("OnDragDetected"))
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	dragObject->SetVisibility(ESlateVisibility::Visible);

	FScriptDelegate onDrop;
	onDrop.BindUFunction(this, TEXT("NativeOnDrop"));
	dragDrop = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	dragDrop->OnDrop.Add(onDrop);
	dragDrop->Payload = dragObject;
	dragDrop->DefaultDragVisual = dragObject;
	dragDrop->Pivot = EDragPivot::MouseDown;

	OutOperation = dragDrop;

	isDragged = true;



}

void UItemIcon::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragLeave"))
	//dragObject->SetVisibility(ESlateVisibility::Hidden);
	//TODO:드롭한 포지션에 아이템 재배치 
}

bool UItemIcon::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop"))
	isDragged = false;
	dragObject->SetVisibility(ESlateVisibility::Hidden);
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UItemIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isDragged)
	{
		FVector2D mousePos;
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);
		controllerRef->GetMousePosition(mousePos.X, mousePos.Y);
		mousePos /= scale;
		//dragDrop->Offset = mousePos;
		dragObjectSlot->SetPosition(mousePos);
	}
}