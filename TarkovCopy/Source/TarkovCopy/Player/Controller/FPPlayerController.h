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
	virtual void SetupInputComponent();
	virtual void BeginPlay() override;
public:
	virtual void PlayerTick(float DeltaTime);

//Created
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> healthHudWidget;
	UPROPERTY()
	UUserWidget* healthHud;
	UPROPERTY()
	UWidget* healthHudBg;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> crosshairWidget;
	UPROPERTY()
	UUserWidget* crosshair;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> alertHudWidget;
	UPROPERTY()
	UUserWidget* alertHud;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> exfilAlertWidget;
	UPROPERTY()
	UUserWidget* exfilAlert;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youreDeadWidget;
	UPROPERTY()
	UUserWidget* youreDead;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youveEscapedWidget;
	UPROPERTY()
	UUserWidget* youveEscaped;


	//TODO: UI관련, 추 후에 컨트롤러로 옮길것
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> inventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemIcon> iconWidget;
	UPROPERTY()
	UUserWidget* inventory;

	UPROPERTY()
	UVerticalBox* itemDetailPanel;
	UPROPERTY()
	TArray<UItemIcon*> items;

	UPROPERTY()
	AEscapeGameMode* gameMode;

	UPROPERTY()
	UTextBlock* alertType;
	UPROPERTY()
	UTextBlock* missionObject;
	UPROPERTY()
	UTextBlock* range;

	UPROPERTY()
	UItemIcon* currentActiveItemIcon;



	//Exfil관련
	bool isExfiling = false;
	UPROPERTY()
	UTextBlock* exfilTimer;
	float timeToExfil = 5.f;
	float exfilCounter = 0.f;


	void CloseAlert();

	void OpenInventory();
	void CloseInventory();
	
public:
	bool isInventoryOpened= false;
	UPROPERTY()
	UCanvasPanel* itemContainer;

	void InitInvenotry();
	void OpenCloseInventory();
	void OpenItemDetailPanel(UItemIcon* pItemIcon);

	void AddItem(UItemInfo* itemInfo,UInventory* pInvenRef);
	void AddPrimary(TSubclassOf<ABaseGun> pWeaponClass);
	void AddSecondary(TSubclassOf<ABaseGun> pWeaponClass);

	void SetADS();
	void SetHipfire();

	void RemovePrimary();
	void RemoveSecondary();

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
