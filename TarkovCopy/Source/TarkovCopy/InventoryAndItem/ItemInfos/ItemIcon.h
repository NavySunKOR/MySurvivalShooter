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
/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UItemIcon : public UUserWidget
{
	GENERATED_BODY()

private:
	bool isDragged;
	
public:
	UPROPERTY()
	UItemInfo* itemInfo;
	UPROPERTY()
	UImage* iconImage;
	UPROPERTY()
	UInventory* invenRef;
	UPROPERTY()
	AFPPlayerController* controllerRef;

	void Init(UItemInfo* pItemInfo, UInventory* pInven, AFPPlayerController* pController);
	UFUNCTION(BlueprintCallable)
	void OpenDetailPanel();

	UFUNCTION(BlueprintCallable)
	void UseItem();
	UFUNCTION(BlueprintCallable)
	void DropItem();


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

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
