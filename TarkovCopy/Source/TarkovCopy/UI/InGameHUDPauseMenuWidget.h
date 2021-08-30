// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUDPauseMenuWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAntiAliasingType : uint8
{
	None =0,
	FXAA,
	MSAA,
	TAA
};

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

	//Blueprint Callables
	UFUNCTION(BlueprintCallable)
	void OnClick_Resume();
	UFUNCTION(BlueprintCallable)
	void OnClick_ExitGame();
	UFUNCTION(BlueprintCallable)
	void OnClick_OpenCloseOptionMenu();

	//Blueprint Callables in optioni
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionToPause();
	UFUNCTION(BlueprintCallable)
	void OnClick_PauseToOption();
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphics();
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsBloomOnOff(bool pIsOn);
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsLensFlareOnOff(bool pIsOn);
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsMotionBlurOnOff(bool pIsOn);
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsAutoExposureOnOff(bool pIsOn);
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsAmbientOcclusionOnOff(bool pIsOn);
	UFUNCTION(BlueprintCallable)
	void OnClick_OptionGraphicsSetAntiAliasing(EAntiAliasingType pType);

public:
	UFUNCTION(BlueprintCallable)
	void OpenClosePauseMenu();

};
