// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 * 
 */
class APlayerCharacter;
class AInGameHUD;
class ABaseGun;
class AEscapeGameMode;
class FSlateRect;
class UCanvasPanel;
class UCanvasPanelSlot;
class UTextBlock;
class UWidget;
class UItemHelmet;
class UItemInfo;
class UItemIcon;
class UInventory;
class UTextBlock;
class UVerticalBox;
class UUserWidget;

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
	AInGameHUD* IngameHud;
	//Status Render

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
	void UpdateInventoryUI();
	void Dead();

	//퀘스트 관련
	void ShowQuestInfo(FString itemName, float distance);
	void ShowExfilPoints(FString exfilPointsName, float distance);
	void ShowCannotExfil();
	void Exfiling();
	void ExfilingComplete();
	void CancelExfiling();

	//스탯 관련
	void HealPlayer(float pHealAmount);

	//파일저장
	void SaveEquipments();

	//UI관련
	FORCEINLINE AInGameHUD* GetInGameHUD() { return IngameHud; };

	//BlueprintUsable
	UFUNCTION(BlueprintCallable)
	void UseCurrentActiveItem();
	UFUNCTION(BlueprintCallable)
	void DiscardCurrentActiveItem();



};
