// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "InGameHUDWidget.h"
#include <Blueprint/UserWidget.h>

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if(IngameHud ==nullptr)
		IngameHud = CreateWidget<UInGameHUDWidget>(GetOwningPlayerController(), IngameHudWidgetClass);
	IngameHud->AddToViewport(0);
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