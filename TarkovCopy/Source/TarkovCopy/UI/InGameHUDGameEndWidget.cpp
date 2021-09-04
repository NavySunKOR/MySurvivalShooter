// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDGameEndWidget.h"
#include <Components/TextBlock.h>

void UInGameHUDGameEndWidget::ShowDeadUI()
{
	YoureDeadText->SetVisibility(ESlateVisibility::Visible);
}

void UInGameHUDGameEndWidget::ShowEscapedUI()
{

	YouveEscapedText->SetVisibility(ESlateVisibility::Visible);
}
