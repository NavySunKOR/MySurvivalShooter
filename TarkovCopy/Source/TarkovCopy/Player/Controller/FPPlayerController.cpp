// Fill out your copyright notice in the Description page of Project Settings.

#include <GameFramework/PlayerInput.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetTree.h>
#include <Blueprint/UserWidget.h>
#include <Components/CanvasPanelSlot.h>
#include <TimerManager.h>
#include "TarkovCopy/GameMode/EscapeGameMode.h"
#include "TarkovCopy/PublicProperty/UMGPublicProperites.h"
#include "FPPlayerController.h"

void AFPPlayerController::BeginPlay()
{

	healthHud = CreateWidget<UUserWidget>(this, healthHudWidget);
	healthHud->AddToViewport();
	healthHudBg = healthHud->GetWidgetFromName(TEXT("Splash"));

	crosshair = CreateWidget<UUserWidget>(this, crosshairWidget);
	crosshair->AddToViewport();
	
	alertHud = CreateWidget<UUserWidget>(this, alertHudWidget);
	alertHud->AddToViewport();

	exfilAlert = CreateWidget<UUserWidget>(this, exfilAlertWidget);
	exfilAlert->AddToViewport();

	youreDead = CreateWidget<UUserWidget>(this, youreDeadWidget);
	youreDead->AddToViewport(2);

	youveEscaped = CreateWidget<UUserWidget>(this, youveEscapedWidget);
	youveEscaped->AddToViewport(2);


	alertHud->SetVisibility(ESlateVisibility::Hidden);
	exfilAlert->SetVisibility(ESlateVisibility::Hidden);
	youveEscaped->SetVisibility(ESlateVisibility::Hidden);
	youreDead->SetVisibility(ESlateVisibility::Hidden);

	alertType = Cast<UTextBlock>(alertHud->GetWidgetFromName(TEXT("AlertType")));
	missionObject = Cast<UTextBlock>(alertHud->GetWidgetFromName(TEXT("Object")));
	range = Cast<UTextBlock>(alertHud->GetWidgetFromName(TEXT("Range")));
	exfilTimer = Cast<UTextBlock>(exfilAlert->GetWidgetFromName(TEXT("ExfilTimer")));

	AEscapeGameMode* gameMode = GetWorld()->GetAuthGameMode<AEscapeGameMode>();
	if(gameMode)
		gameMode->SelectQuestItems();

}

void AFPPlayerController::SetupInputComponent()
{

}




void AFPPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (isExfiling)
	{
		exfilCounter += DeltaTime;
		exfilTimer->SetText(FText::FromString(FString::FormatAsNumber(timeToExfil - exfilCounter)));
		if ((timeToExfil - exfilCounter) <= 0.f)
		{
			ExfilingComplete();
		}
	}
}

void AFPPlayerController::InitInvenotry()
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());

	if (inventory)
		inventory->RemoveFromViewport();

	inventory = CreateWidget<UUserWidget>(this, inventoryWidget);

	itemContainer = Cast<UCanvasPanel>(inventory->GetWidgetFromName(TEXT("ItemContainer")));
	if (itemContainer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gone burst!"));
	}
	UCanvasPanelSlot* backgroundImage  = Cast<UCanvasPanelSlot>(inventory->GetWidgetFromName(TEXT("Background"))->Slot);
	if (backgroundImage != nullptr && itemContainer != nullptr)
	{
		backgroundImage->SetSize(FVector2D(UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) * character->inventory->backpack->GetBackpackSize());
		UCanvasPanelSlot* canva = Cast<UCanvasPanelSlot>(itemContainer->Slot);
		if (canva != nullptr)
			canva->SetSize(FVector2D(UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) * character->inventory->backpack->GetBackpackSize());
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Canvas is null!"));

}

void AFPPlayerController::OpenCloseInventory()
{
	if (inventory->IsInViewport())
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		SetIgnoreLookInput(false);
		SetIgnoreMoveInput(false);
		isInventoryOpened = false;
		inventory->RemoveFromViewport();

		UE_LOG(LogTemp, Warning, TEXT("close"))
	}
	else
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		isInventoryOpened = true;
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
		inventory->AddToViewport();

		UE_LOG(LogTemp, Warning, TEXT("open"))
	}
}

void AFPPlayerController::AddItem(UItemInfo* itemInfo, UInventory* pInvenRef)
{
	if (inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fucking inven"))
			return;
	}
	if (inventory->WidgetTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fucking WidgetTree"))
			return;
	}

	if (iconWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fucking iconWidget"))
		return;
	}

	UItemIcon* uiItem = inventory->WidgetTree->ConstructWidget<UItemIcon>(iconWidget); //TODO:WidgetTree가 계속 널이 되는 경우가 있는데 이 문제를 해결해야됨.
	UCanvasPanelSlot* panelSlotForItem = Cast<UCanvasPanelSlot>(itemContainer->AddChild(uiItem));

	uiItem->Slot = panelSlotForItem;
	uiItem->Init(itemInfo, pInvenRef, this);
	items.Add(uiItem);
}

void AFPPlayerController::AddPrimary(TSubclassOf<ABaseGun> pWeaponClass)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());
	if (character != nullptr)
	{
		character->AddPrimary(pWeaponClass);
	}
}

void AFPPlayerController::AddSecondary(TSubclassOf<ABaseGun> pWeaponClass)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());
	if (character != nullptr)
	{
		character->AddSecondary(pWeaponClass);
	}
}

void AFPPlayerController::SetADS()
{
	crosshair->SetVisibility(ESlateVisibility::Hidden);
}

void AFPPlayerController::SetHipfire()
{
	crosshair->SetVisibility(ESlateVisibility::Visible);
}


void AFPPlayerController::RemovePrimary()
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());
	if (character != nullptr)
	{
		character->RemovePrimary();
	}
}

void AFPPlayerController::RemoveSecondary()
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());
	if (character != nullptr)
	{
		character->RemoveSecondary();
	}
}

void AFPPlayerController::HealPlayer(float pHealAmount)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(GetPawn());
	if (character != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UsingHealingItem"));
		float curHp = character->HealPlayer(pHealAmount);
		UpdateHealthHud(curHp);
	}
}

void AFPPlayerController::UpdateHealthHud(float pCurHealth)
{
	float opacityAmount = 1.f - pCurHealth / 100.f;
	if (opacityAmount < 0.2f)
		opacityAmount = 0.2f;
	healthHudBg->SetRenderOpacity(opacityAmount);
}

void CleanupArray(TArray<UItemIcon*>& pItem)
{
	for (int i = pItem.Num() - 1; i >= 0; i--)
	{
		if (pItem[i] == nullptr)
		{
			pItem.RemoveAt(i);
		}
	}
}

void AFPPlayerController::UpdateInventoryUI()
{
	TArray<UItemIcon*> tempItems = items;

	for (int i = tempItems.Num() - 1; i >= 0; i--)
	{
		if (tempItems[i]->itemInfo == nullptr || tempItems[i]->itemInfo->currentCapacity == 0)
		{
			tempItems[i]->RemoveFromParent();
			tempItems[i] = nullptr;
		}
	}

	CleanupArray(tempItems);

	items = tempItems;
}

void AFPPlayerController::Dead()
{
	youreDead->SetVisibility(ESlateVisibility::Visible);
	//TODO:메인화면으로 가는거 넣을것.
}

void AFPPlayerController::ShowQuestInfo(FString itemName, float distance)
{
	alertHud->SetVisibility(ESlateVisibility::Visible);
	alertType->SetText(FText::FromString("Quest"));

	FString objectMessage = "Get the ";
	objectMessage.Append(itemName);
	missionObject->SetText(FText::FromString(objectMessage));

	FString rangeText = FString::FormatAsNumber((int)distance);
	rangeText.Append("m");
	range->SetText(FText::FromString(rangeText));

	FTimerHandle timers;
	GetWorldTimerManager().SetTimer(timers,this,&AFPPlayerController::CloseAlert, 3.f, false);
}

void AFPPlayerController::ShowExfilPoints(FString exfilPointsName, float distance)
{
	alertHud->SetVisibility(ESlateVisibility::Visible);

	alertType->SetText(FText::FromString("Exfil"));

	FString objectMessage = "Get to the ";
	objectMessage.Append(exfilPointsName);
	missionObject->SetText(FText::FromString(objectMessage));

	FString rangeText = FString::FormatAsNumber((int)distance);
	rangeText.Append("m");
	range->SetText(FText::FromString(rangeText));

	FTimerHandle timers;
	GetWorldTimerManager().SetTimer(timers, this, &AFPPlayerController::CloseAlert, 3.f, false);
}


void AFPPlayerController::ShowCannotExfil()
{
	UE_LOG(LogTemp, Warning, TEXT("Can not exfil!"));
}

void AFPPlayerController::Exfiling()
{
	if (!isExfiling)
	{
		isExfiling = true;
		exfilCounter = 0.f;
		exfilAlert->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Exfiling......"));
	}
}

void AFPPlayerController::ExfilingComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("ExfilingComplete"));
	isExfiling = false;
	exfilAlert->SetVisibility(ESlateVisibility::Hidden);
	youveEscaped->SetVisibility(ESlateVisibility::Visible);
	//TODO:메인화면으로 옮기는 작업 넣을것
}

void AFPPlayerController::CancelExfiling()
{
	if (isExfiling)
	{
		isExfiling = false;
		exfilAlert->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("Cancel exfiling....."));
	}
}

void AFPPlayerController::CloseAlert()
{
	alertHud->SetVisibility(ESlateVisibility::Hidden);
}
