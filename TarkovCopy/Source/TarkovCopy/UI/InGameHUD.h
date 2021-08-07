// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
class UInGameHUDWidget;
class UInGameHUDTopRightWidget;
class UInGameHUDPauseMenuWidget;
UCLASS()
class TARKOVCOPY_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameHUDWidget> IngameHudWidgetClass;
	UPROPERTY()
	UInGameHUDWidget* IngameHud;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameHUDTopRightWidget> IngameHudTopRightWidgetClass;
	UPROPERTY()
	UInGameHUDTopRightWidget* IngameHudTopRight;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameHUDPauseMenuWidget> IngameHudPauseMenuClass;
	UPROPERTY()
	UInGameHUDPauseMenuWidget* IngameHudPauseMenu;

public:
	bool IsPauseMenuOpened();
	void OpenClosePauseMenu();
	void LockInput();
	void UnlockInput();

	void UpdateHealthHud(float pCurHealth);
	void ShowHitIndicator(FVector pHitDir);
	void GetFlashed(float pFlashTime, FVector pFlashbangPos);
	void SetCrosshairVisible();
	void SetCrosshairInvisible();

	void ShowQuestInfo(FString itemName, float distance);
	void ShowExfilPoints(FString exfilPointsName, float distance);

};
