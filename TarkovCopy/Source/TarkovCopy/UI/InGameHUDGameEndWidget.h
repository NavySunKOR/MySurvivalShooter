// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUDGameEndWidget.generated.h"

/**
 * 
 */
class UTextBlock; 
UCLASS()
class TARKOVCOPY_API UInGameHUDGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* YoureDeadText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* YouveEscapedText;

public:
	void ShowDeadUI();
	void ShowEscapedUI();
};
