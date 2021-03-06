// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include "ItemIcon.generated.h"

class UItemInfo;
class UInventory;
class AFPPlayerController;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemIcon : public UUserWidget
{
	GENERATED_BODY()

private:
	bool isDragged;
	void AdjustPositionRectForItemContainer(FSlateRect& positionIntRect, FSlateRect itemContainerIntRect);
	
public:
	UPROPERTY()
	UItemInfo* itemInfo;
	UPROPERTY()
	UImage* iconImage;
	UPROPERTY()
	UInventory* invenRef;
	UPROPERTY()
	AFPPlayerController* controllerRef;
	UPROPERTY()
	UCanvasPanelSlot* canvasSlot;

	void Init(UItemInfo* pItemInfo, UInventory* pInven, AFPPlayerController* pController);
	void OpenDetailPanel();

	UFUNCTION(BlueprintCallable)
	void UseItem();
	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintPure)
	FString GetAmountText() const;


	//Item Drag operation
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> origin;
	UPROPERTY()
	UUserWidget* dragObject;
	UPROPERTY()
	UImage* dragObjectImage;
	UPROPERTY()
	UCanvasPanelSlot* dragObjectSlot;
	UPROPERTY()
	UDragDropOperation* dragDrop;
	UPROPERTY()
	FPointerEvent handledDragDrop;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	void OnDropAction(FVector2D lastMousePosition);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
