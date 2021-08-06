// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDTopRightWidget.h"


void UInGameHUDTopRightWidget::SetMessages(FText pAlertType, FText pObjectText, FText pRangeText)
{
	AlertTypeTextBlock->SetText(pAlertType);
	ObjectTextBlock->SetText(pObjectText);
	RangeTextBlock->SetText(pRangeText);
}

void UInGameHUDTopRightWidget::ShowQuestInfo(FString itemName, float distance)
{
	TopRightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);

	FString objectMessage = "Get the ";
	objectMessage.Append(itemName);

	FString rangeString = FString::FormatAsNumber((int)distance);
	rangeString.Append("m");

	SetMessages(FText::FromString("Quest"), FText::FromString(objectMessage), FText::FromString(rangeString));

	GetWorld()->GetTimerManager().ClearTimer(CloseTimer);
	GetWorld()->GetTimerManager().SetTimer(CloseTimer, this, &UInGameHUDTopRightWidget::CloseAlert, 3.f, false);
}

void UInGameHUDTopRightWidget::ShowExfilPoints(FString exfilPointsName, float distance)
{
	TopRightOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);

	FString objectMessage = "Get to the ";
	objectMessage.Append(exfilPointsName);

	FString rangeString = FString::FormatAsNumber((int)distance);
	rangeString.Append("m");

	SetMessages(FText::FromString("Exfil"), FText::FromString(objectMessage), FText::FromString(rangeString));

	GetWorld()->GetTimerManager().ClearTimer(CloseTimer);
	GetWorld()->GetTimerManager().SetTimer(CloseTimer, this, &UInGameHUDTopRightWidget::CloseAlert, 3.f, false);
}

void UInGameHUDTopRightWidget::CloseAlert()
{
	TopRightOverlay->SetVisibility(ESlateVisibility::Hidden);
}


//void AFPPlayerController::ShowQuestInfo(FString itemName, float distance)
//{
//	alertHudUI->SetVisibility(ESlateVisibility::Visible);
//	alertTypeText->SetText(FText::FromString("Quest"));
//
//	FString objectMessage = "Get the ";
//	objectMessage.Append(itemName);
//	missionObjectText->SetText(FText::FromString(objectMessage));
//
//	FString rangeString = FString::FormatAsNumber((int)distance);
//	rangeString.Append("m");
//	rangeText->SetText(FText::FromString(rangeString));
//
//	FTimerHandle timers;
//	GetWorldTimerManager().SetTimer(timers, this, &AFPPlayerController::CloseAlert, 3.f, false);
//}
//
//void AFPPlayerController::ShowExfilPoints(FString exfilPointsName, float distance)
//{
//	alertHudUI->SetVisibility(ESlateVisibility::Visible);
//
//	alertTypeText->SetText(FText::FromString("Exfil"));
//
//	FString objectMessage = "Get to the ";
//	objectMessage.Append(exfilPointsName);
//	missionObjectText->SetText(FText::FromString(objectMessage));
//
//	FString rangeString = FString::FormatAsNumber((int)distance);
//	rangeString.Append("m");
//	rangeText->SetText(FText::FromString(rangeString));
//
//	FTimerHandle timers;
//	GetWorldTimerManager().SetTimer(timers, this, &AFPPlayerController::CloseAlert, 3.f, false);
//}
//
//void AFPPlayerController::CloseAlert()
//{
//	alertHudUI->SetVisibility(ESlateVisibility::Hidden);
//}
