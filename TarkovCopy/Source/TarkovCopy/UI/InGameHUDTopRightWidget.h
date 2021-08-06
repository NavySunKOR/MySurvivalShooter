// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include "InGameHUDTopRightWidget.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UInGameHUDTopRightWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(Meta = (BindWidget))
	UOverlay* TopRightOverlay = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* AlertTypeTextBlock = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ObjectTextBlock = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* RangeTextBlock = nullptr;

	FTimerHandle CloseTimer;

	void SetMessages(FText pAlertType, FText pObjectText, FText pRangeText);

	void CloseAlert();
public:
	void ShowQuestInfo(FString itemName, float distance);
	void ShowExfilPoints(FString exfilPointsName, float distance);
	
};
