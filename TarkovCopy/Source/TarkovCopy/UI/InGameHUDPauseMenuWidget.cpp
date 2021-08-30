// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDPauseMenuWidget.h"
#include "TarkovCopy/UI/InGameHUD.h"
#include <Components/CanvasPanel.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UInGameHUDPauseMenuWidget::OpenClosePauseMenu()
{
	if (OptionMenuPanel->IsVisible())
	{
		CloseOptionMenu();
		ClosePauseMenu();
	}
}

void UInGameHUDPauseMenuWidget::OnClick_OpenCloseOptionMenu()
{
	if (OptionMenuPanel->IsVisible())
	{
		CloseOptionMenu();
	}
	else
	{
		OpenOptionMenu();
	}
}

void UInGameHUDPauseMenuWidget::OnClick_OptionToPause()
{
	OpenPauseMenu();
	CloseOptionMenu();
}

void UInGameHUDPauseMenuWidget::OnClick_PauseToOption()
{
	ClosePauseMenu();
	OpenOptionMenu();
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphics()
{
	//TODO:다중 탭이 있다면 그래픽 옵션의 패널을 열것
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsBloomOnOff(bool pIsOn)
{
	if (!pIsOn)
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.Bloom 0"));
	else
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.Bloom 1"));
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsLensFlareOnOff(bool pIsOn)
{
	if (!pIsOn)
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.LensFlare 0"));
	else
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.LensFlare 1"));
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsMotionBlurOnOff(bool pIsOn)
{
	if (!pIsOn)
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.MotionBlur 0"));
	else
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.MotionBlur 1"));
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsAutoExposureOnOff(bool pIsOn)
{
	if (!pIsOn)
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AutoExposure 0"));
	else
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AutoExposure 1"));
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsAmbientOcclusionOnOff(bool pIsOn)
{
	if (!pIsOn)
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AmbientOcclusion 0"));
	else
		GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AmbientOcclusion 1"));
}

void UInGameHUDPauseMenuWidget::OnClick_OptionGraphicsSetAntiAliasing(EAntiAliasingType pType)
{
	switch (pType)
	{
		case EAntiAliasingType::None:
			GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AntiAliasing 0"));
			break;
		case EAntiAliasingType::FXAA:
			GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AntiAliasing 1"));
			break;
		case EAntiAliasingType::MSAA:
			GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AntiAliasing 3"));
			break;
		case EAntiAliasingType::TAA:
			GetOwningPlayer()->ConsoleCommand(TEXT("r.DefaultFeature.AntiAliasing 2"));
			break;
		default:
			break;
	}
}

void UInGameHUDPauseMenuWidget::OnClick_Resume()
{
	AInGameHUD* IngameHud = Cast<AInGameHUD>(GetOwningPlayer()->GetHUD());
	if(IngameHud)
		IngameHud->OpenClosePauseMenu();
}

void UInGameHUDPauseMenuWidget::OnClick_ExitGame()
{
	UWidgetBlueprintLibrary::SetInputMode_UIOnly(GetOwningPlayer());
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
void UInGameHUDPauseMenuWidget::OpenPauseMenu()
{
	PauseMenuPanel->SetVisibility(ESlateVisibility::Visible);
}
void UInGameHUDPauseMenuWidget::ClosePauseMenu()
{
	PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameHUDPauseMenuWidget::OpenOptionMenu()
{
	ClosePauseMenu();
	OptionMenuPanel->SetVisibility(ESlateVisibility::Visible);
}

void UInGameHUDPauseMenuWidget::CloseOptionMenu()
{
	OptionMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	OpenPauseMenu();
}
