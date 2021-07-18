// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Overlay.h>
#include <Components/BackgroundBlur.h>
#include <Components/Image.h>
#include "InGameHUDWidget.generated.h"
#define PROCESS_FRAMERATE 0.016f


/**
 * 
 */


UCLASS()
class TARKOVCOPY_API UInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FTimerManager& GetWorldTimer();
#pragma region Flashbang
	FTimerHandle FlashTimerHandle;
	float CurFlashTime = 0.f;
	float FlashFadeStartTime = 0.f;
	float FlashFadeAmount = 0.f;
	float FlashInterval = 0.f;
	void FlashProcessing();
#pragma endregion

#pragma region HitIndicator
	FTimerHandle HitIndicatorTimerHandle;
	FVector HitFromPos;
	float CurHitIndicatorTime;
	float HitIndicatorFadeAmount;
	float HitIndicatorFadeStartTime;
	const float HitIndicatorInterval = 4.f; //이 숫자는 상수

	void IndicatorProcessing();
#pragma endregion

protected:
	UPROPERTY(Meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(Meta = (BindWidget))
	UOverlay* HitIndicate;

	UPROPERTY(Meta = (BindWidget))
	UBackgroundBlur* DamageSplash;

	UPROPERTY(Meta = (BindWidget))
	UBackgroundBlur* FlashSplash;

public:

	void UpdateHealthHud(float pCurHealth);
	void ShowHitIndicator(FVector pHitDir);
	void GetFlashed(float pFlashTime, FVector pFlashbangPos);
	void SetCrosshairVisible();
	void SetCrosshairInvisible();


};