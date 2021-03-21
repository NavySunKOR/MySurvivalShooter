// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include <TarkovCopy/InventoryAndItem/ItemInfos/ItemIcon.h>
#include <TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h>
#include "FPPlayerController.generated.h"

/**
 * 
 */
class AEscapeGameMode;
class UVerticalBox;
UCLASS()
class TARKOVCOPY_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
//Generated
protected: 
	virtual void BeginPlay() override;
public:
	virtual void PlayerTick(float DeltaTime);

//Created
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> healthHudWidget;
	UPROPERTY()
	UUserWidget* healthHudUI;
	UPROPERTY()
	UWidget* healthHudBgUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> crosshairWidget;
	UPROPERTY()
	UUserWidget* crosshairUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> alertHudWidget;
	UPROPERTY()
	UUserWidget* alertHudUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> exfilAlertWidget;
	UPROPERTY()
	UUserWidget* exfilAlertUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youreDeadWidget;
	UPROPERTY()
	UUserWidget* youreDeadUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youveEscapedWidget;
	UPROPERTY()
	UUserWidget* youveEscapedUI;


	//TODO: UI관련, 추 후에 컨트롤러로 옮길것
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> inventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemIcon> iconWidget;


	UPROPERTY()
	UUserWidget* inventoryUI;
	UPROPERTY()
	UCanvasPanel* itemContainerUI;
	UPROPERTY()
	UCanvasPanelSlot* itemContainerUISlot;
	UPROPERTY()
	UCanvasPanelSlot* primaryWeaponContainerUISlot;
	UPROPERTY()
	UCanvasPanelSlot* secondaryWeaponContainerUISlot;

	UPROPERTY()
	UCanvasPanel* primaryWeaponContainerUI;
	UPROPERTY()
	UCanvasPanel* secondaryWeaponContainerUI;

	UPROPERTY()
	UVerticalBox* itemDetailPanel;
	UPROPERTY()
	TArray<UItemIcon*> items;

	UPROPERTY()
	AEscapeGameMode* gameMode;

	UPROPERTY()
	UTextBlock* alertTypeText;
	UPROPERTY()
	UTextBlock* missionObjectText;
	UPROPERTY()
	UTextBlock* rangeText;
	UPROPERTY()
	UTextBlock* exfilTimerText;

	UPROPERTY()
	UItemIcon* currentActiveItemIcon;

	UPROPERTY()
	APlayerCharacter* ownerPlayerCharacter;


	//Exfil관련
	bool isExfiling = false;
	float timeToExfil = 5.f;
	float exfilCounter = 0.f;


	void CloseAlert();

	void OpenInventory();
	void CloseInventory();
	
public:
	bool isInventoryOpened= false;
	FSlateRect itemContainerRect;
	FSlateRect primaryWeaponContainerRect;
	FSlateRect secondaryWeaponContainerRect;

	void InitInvenotry();
	void OpenCloseInventory();
	void OpenItemDetailPanel(UItemIcon* pItemIcon);

	//Item
	//인터페이스로 사용
	void AddItem(UItemInfo* itemInfo,UInventory* pInvenRef);
	//인터페이스로 사용
	void DropItem(UItemIcon* pItemIcon);
	//인터페이스로 사용
	void StartMoveItemPos(UItemInfo* pItemInfo);
	//인터페이스로 사용
	bool CanItemMoveTo(FSlateRect pIntSlateRect);
	//인터페이스로 사용
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	//인터페이스로 사용
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	bool IsInItemContainer(UItemIcon* pItemInfo) const;
	bool IsInEquipmentSlot(UItemIcon* pItemInfo) const;

	//인터페이스로 사용
	void AddPrimary(TSubclassOf<ABaseGun> pWeaponClass,UItemWeapon* pItemWeapon);
	//인터페이스로 사용
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponClass, UItemWeapon* pItemWeapon);

	void SetADS();
	void SetHipfire();

	//인터페이스로 사용
	void RemovePrimary();
	//인터페이스로 사용
	void RemoveSecondary();

	//인터페이스로 사용
	void HealPlayer(float pHealAmount);
	void UpdateHealthHud(float pCurHealth);
	void UpdateInventoryUI();

	void Dead();

	void ShowQuestInfo(FString itemName, float distance);
	void ShowExfilPoints(FString exfilPointsName, float distance);
	void ShowCannotExfil();
	void Exfiling();
	void ExfilingComplete();
	void CancelExfiling();



	//BlueprintUsable
	UFUNCTION(BlueprintCallable)
	void UseCurrentActiveItem();
	UFUNCTION(BlueprintCallable)
	void DiscardCurrentActiveItem();



};
