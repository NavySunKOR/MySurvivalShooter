// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "InGameHUDWidget.h"
#include "InGameHUDTopRightWidget.h"
#include "InGameHUDPauseMenuWidget.h"
#include <Blueprint/UserWidget.h>

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
}

bool AInGameHUD::IsPauseMenuOpened()
{
	return IngameHudPauseMenu->IsInViewport();
}

void AInGameHUD::OpenClosePauseMenu()
{
	if (IngameHudPauseMenu->IsInViewport())
	{
		IngameHudPauseMenu->OpenClosePauseMenu();
		IngameHudPauseMenu->RemoveFromViewport();
		UnlockInput();
	}
	else
	{
		IngameHudPauseMenu->AddToViewport(0);
		IngameHudPauseMenu->OpenClosePauseMenu();
		LockInput();
	}

}

void AInGameHUD::LockInput()
{
	PlayerOwner->bShowMouseCursor = true;
	PlayerOwner->bEnableClickEvents = true;
	PlayerOwner->bEnableMouseOverEvents = true;
	PlayerOwner->SetIgnoreLookInput(true);
	PlayerOwner->SetIgnoreMoveInput(true);
}

void AInGameHUD::UnlockInput()
{
	PlayerOwner->bShowMouseCursor = false;
	PlayerOwner->bEnableClickEvents = false;
	PlayerOwner->bEnableMouseOverEvents = false;
	PlayerOwner->SetIgnoreLookInput(false);
	PlayerOwner->SetIgnoreMoveInput(false);
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
