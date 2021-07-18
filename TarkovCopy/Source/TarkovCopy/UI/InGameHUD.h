// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
class UInGameHUDWidget;
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
public:
	void UpdateHealthHud(float pCurHealth);
	void ShowHitIndicator(FVector pHitDir);
	void GetFlashed(float pFlashTime, FVector pFlashbangPos);
	void SetCrosshairVisible();
	void SetCrosshairInvisible();

};
