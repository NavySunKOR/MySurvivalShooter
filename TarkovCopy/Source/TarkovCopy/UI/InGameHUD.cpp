// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "InGameHUDWidget.h"
#include "InGameHUDTopRightWidget.h"
#include "InGameHUDPauseMenuWidget.h"
#include <Blueprint/UserWidget.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (IngameHud == nullptr)
		IngameHud = CreateWidget<UInGameHUDWidget>(GetOwningPlayerController(), IngameHudWidgetClass);
	IngameHud->AddToViewport(0);


	if (IngameHudTopRight == nullptr)
		IngameHudTopRight = CreateWidget<UInGameHUDTopRightWidget>(GetOwningPlayerController(), IngameHudTopRightWidgetClass);
	IngameHudTopRight->AddToViewport(0);

	if(IngameHudPauseMenu == nullptr)
		IngameHudPauseMenu = CreateWidget<UInGameHUDPauseMenuWidget>(GetOwningPlayerController(), IngameHudPauseMenuClass);

	IngameHudPauseMenu->AddToViewport(0);
	IngameHudPauseMenu->SetVisibility(ESlateVisibility::Hidden);
}

bool AInGameHUD::IsPauseMenuOpened()
{
	return IngameHudPauseMenu->IsInViewport();
}

void AInGameHUD::OpenClosePauseMenu()
{
	if (IngameHudPauseMenu->IsVisible())
	{
		IngameHudPauseMenu->SetVisibility(ESlateVisibility::Hidden);
		IngameHudPauseMenu->OpenClosePauseMenu();
		UnlockInput();
	}
	else
	{
		IngameHudPauseMenu->SetVisibility(ESlateVisibility::Visible);
		IngameHudPauseMenu->OpenClosePauseMenu();
		LockInput();
	}

}

void AInGameHUD::LockInput()
{
	GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());
}

void AInGameHUD::UnlockInput()
{
	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
}

void AInGameHUD::UpdateHealthHud(float pCurHealth)
{
	IngameHud->UpdateHealthHud(pCurHealth);
}

void AInGameHUD::ShowHitIndicator(FVector pHitDir)
{
	IngameHud->ShowHitIndicator(pHitDir);
}

void AInGameHUD::GetFlashed(float pFlashTime, FVector pFlashbangPos)
{
	IngameHud->GetFlashed(pFlashTime, pFlashbangPos);
}

void AInGameHUD::SetCrosshairVisible()
{
	IngameHud->SetCrosshairVisible();
}

void AInGameHUD::SetCrosshairInvisible()
{
	IngameHud->SetCrosshairInvisible();
}

void AInGameHUD::ShowQuestInfo(FString itemName, float distance)
{
	IngameHudTopRight->ShowQuestInfo(itemName, distance);
}

void AInGameHUD::ShowExfilPoints(FString exfilPointsName, float distance)
{
	IngameHudTopRight->ShowExfilPoints(exfilPointsName, distance);
}

void AInGameHUD::ShowGameResult(EGameEndType pEnum)
{

}

void AInGameHUD::ShowExfilStatus()
{
}

void AInGameHUD::HideExfilStatus()
{

}
