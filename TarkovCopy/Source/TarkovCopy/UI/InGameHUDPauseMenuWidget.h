// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUDPauseMenuWidget.generated.h"

/**
 * 
 */
class UCanvasPanel;
UCLASS()
class TARKOVCOPY_API UInGameHUDPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* PauseMenuPanel;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* OptionMenuPanel;

	void OpenPauseMenu();
	void ClosePauseMenu();
	void OpenOptionMenu();
	void CloseOptionMenu();
public:
	void OpenClosePauseMenu();

};
