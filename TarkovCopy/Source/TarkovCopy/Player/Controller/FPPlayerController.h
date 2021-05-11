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
class UItemHelmet;
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
	//Status Render
	UPROPERTY()
	UUserWidget* healthHudUI;
	UPROPERTY()
	UWidget* healthHudBgUI;
	UPROPERTY()
	UWidget* flashHudBgUI;

	//Crosshair Render
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> crosshairWidget;
	UPROPERTY()
	UUserWidget* crosshairUI;


	//Mission and notify
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

	//Hit Indicator
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> hitIndicatorWidget;
	UUserWidget* hitIndicatorUI;
	UWidget* hitIndicatorActualUI;


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
	UCanvasPanel* primaryWeaponContainerUI;
	UPROPERTY()
	UCanvasPanel* secondaryWeaponContainerUI;
	UPROPERTY()
	UCanvasPanel* helmetContainerUI;

	UPROPERTY()
	UCanvasPanelSlot* itemContainerUISlot;
	UPROPERTY()
	UCanvasPanelSlot* primaryWeaponContainerUISlot;
	UPROPERTY()
	UCanvasPanelSlot* secondaryWeaponContainerUISlot;
	UPROPERTY()
	UCanvasPanelSlot* helmetContainerUISlot;

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


	//Exfil관련
	bool isExfiling = false;
	float timeToExfil = 5.f;
	float exfilCounter = 0.f;

	//flash 관련
	bool isFlashed = false;
	float flashTimer = 0.f;
	float flashInterval = 0.f;
	float flashFadeAmount = 0.f;

	//hitIndicator 관련
	bool isHit = false;
	float hitIndicatorTimer = 0.f;
	float hitIndicatorInterval = 4.f;
	float hitIndicatorFadeAmount = 0.f;
	FVector hitFromPos;


	void CloseAlert();

	void OpenInventory();
	void CloseInventory();
	
public:
	bool isInventoryOpened= false;
	FSlateRect itemContainerRect;
	FSlateRect primaryWeaponContainerRect;
	FSlateRect secondaryWeaponContainerRect;
	FSlateRect helmetContainerRect;

	UPROPERTY()
	APlayerCharacter* ownerPlayerCharacter;

	//플레이어의 인터페이스 뚫기
	TArray<UItemInfo*> GetItemContainers();

	void InitInvenotry();
	void OpenCloseInventory();
	void LockOpenUI();
	void UnlockCloseUI();
	void OpenItemDetailPanel(UItemIcon* pItemIcon);

	//Item 이동 관련
	void AddItem(UItemInfo* itemInfo,UInventory* pInvenRef);
	void DropItem(UItemIcon* pItemIcon);
	void StartMoveItemPos(UItemInfo* pItemInfo);
	bool CanItemMoveTo(FSlateRect pIntSlateRect);
	void MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect);
	void FailedToMoveItemPos(UItemInfo* pItemInfo);

	bool IsInItemContainer(UItemIcon* pItemInfo) const;
	bool IsInEquipmentSlot(UItemIcon* pItemInfo) const;

	//무기 관련
	void AddPrimary(TSubclassOf<ABaseGun> pWeaponClass,UItemWeapon* pItemWeapon);
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponClass, UItemWeapon* pItemWeapon);

	void SetADS();
	void SetHipfire();

	void RemovePrimary();
	void RemoveSecondary();

	//방어구 관련
	void AddHelmet(UItemHelmet* pHelmetInfo);
	void RemoveHelmet(UItemHelmet* pHelmetInfo);

	//상태 관련
	void HealPlayer(float pHealAmount);
	void UpdateHealthHud(float pCurHealth);
	void UpdateInventoryUI();
	void Dead();

	//섬광탄
	void GetFlashed(float pFlashTime,FVector pFlashbangPos);

	//피격
	void ShowHitIndicator(FVector pHitDir);

	//퀘스트 관련
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
