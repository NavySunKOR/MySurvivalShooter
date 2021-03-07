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
	UUserWidget* healthHud;
	UWidget* healthHudBg;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> crosshairWidget;
	UUserWidget* crosshair;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> alertHudWidget;
	UUserWidget* alertHud;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> exfilAlertWidget;
	UUserWidget* exfilAlert;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youreDeadWidget;
	UUserWidget* youreDead;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> youveEscapedWidget;
	UUserWidget* youveEscaped;


	//TODO: UI관련, 추 후에 컨트롤러로 옮길것
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> inventoryWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemIcon> iconWidget;
	UPROPERTY()
	UUserWidget* inventory;
	UPROPERTY()
	UCanvasPanel* itemContainer;
	TArray<UItemIcon*> items;


	AEscapeGameMode* gameMode;

	UTextBlock* alertType;
	UTextBlock* missionObject;
	UTextBlock* range;



	//Exfil관련
	bool isExfiling = false;
	UTextBlock* exfilTimer;
	float timeToExfil = 5.f;
	float exfilCounter = 0.f;


	void CloseAlert();

	void OpenInventory();
	void CloseInventory();
	
public:
	bool isInventoryOpened= false;
	void InitInvenotry();
	void OpenCloseInventory();

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



};
