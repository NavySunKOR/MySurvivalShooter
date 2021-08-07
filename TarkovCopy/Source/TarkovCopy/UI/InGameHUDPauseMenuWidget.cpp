// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDPauseMenuWidget.h"
#include <Components/CanvasPanel.h>

void UInGameHUDPauseMenuWidget::OpenClosePauseMenu()
{
	if (OptionMenuPanel->IsVisible())
	{
		CloseOptionMenu();
		ClosePauseMenu();
	}
	else
	{
		if (!PauseMenuPanel->IsVisible())
		{
			//�޴� ����
			OpenPauseMenu();
		}
		else
		{
			//�޴� �ݱ�
			ClosePauseMenu();
		}
	}

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
