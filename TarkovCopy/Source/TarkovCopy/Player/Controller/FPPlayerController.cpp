// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlayerController.h"
#include <GameFramework/PlayerInput.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetTree.h>
#include <Blueprint/UserWidget.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/VerticalBox.h>
#include <TimerManager.h>
#include "TarkovCopy/GameMode/EscapeGameMode.h"
#include "TarkovCopy/PublicProperty/UMGPublicProperites.h"
#include <Blueprint/WidgetLayoutLibrary.h>

void AFPPlayerController::BeginPlay()
{
	bShowMouseCursor = false;

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
	
	gameMode = GetWorld()->GetAuthGameMode<AEscapeGameMode>();
	if(gameMode)
		gameMode->SelectQuestItems();

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
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (inventory)
	{
		inventory->RemoveFromViewport();
		inventory = nullptr;
	}

	inventory = CreateWidget<UUserWidget>(this, inventoryWidget);
	itemDetailPanel = Cast<UVerticalBox>(inventory->GetWidgetFromName(TEXT("DetailPanel")));
	itemContainer = Cast<UCanvasPanel>(inventory->GetWidgetFromName(TEXT("ItemContainer")));
	primaryWeaponContainer = Cast<UCanvasPanel>(inventory->GetWidgetFromName(TEXT("PrimaryWeaponContainer")));
	secondaryWeaponContainer = Cast<UCanvasPanel>(inventory->GetWidgetFromName(TEXT("SecondaryWeaponContainer")));
	
	itemContainerSlot = Cast<UCanvasPanelSlot>(itemContainer->Slot);
	primaryWeaponContainerSlot = Cast<UCanvasPanelSlot>(primaryWeaponContainer->Slot);
	secondaryWeaponContainerSlot = Cast<UCanvasPanelSlot>(secondaryWeaponContainer->Slot);
	
	itemContainerSlot->SetSize(FVector2D(UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) * ownerPlayerCharacter->inventory->backpack->GetBackpackSize());

	itemContainerRect.Left = itemContainerSlot->GetPosition().X;
	itemContainerRect.Top = itemContainerSlot->GetPosition().Y;
	itemContainerRect.Right = itemContainerRect.Left + itemContainerSlot->GetSize().X;
	itemContainerRect.Bottom = itemContainerRect.Top + itemContainerSlot->GetSize().Y;

	primaryWeaponContainerRect.Left = primaryWeaponContainerSlot->GetPosition().X;
	primaryWeaponContainerRect.Top = primaryWeaponContainerSlot->GetPosition().Y;
	primaryWeaponContainerRect.Right = primaryWeaponContainerRect.Left + primaryWeaponContainerSlot->GetSize().X;
	primaryWeaponContainerRect.Bottom = primaryWeaponContainerRect.Top + primaryWeaponContainerSlot->GetSize().Y;

	UE_LOG(LogTemp, Warning, TEXT("primaryWeaponContainerRect : %s"), *primaryWeaponContainerRect.ToString());

	secondaryWeaponContainerRect.Left = secondaryWeaponContainerSlot->GetPosition().X;
	secondaryWeaponContainerRect.Top = secondaryWeaponContainerSlot->GetPosition().Y;
	secondaryWeaponContainerRect.Right = secondaryWeaponContainerRect.Left + secondaryWeaponContainerSlot->GetSize().X;
	secondaryWeaponContainerRect.Bottom = secondaryWeaponContainerRect.Top + secondaryWeaponContainerSlot->GetSize().Y;

	UE_LOG(LogTemp, Warning, TEXT("secondaryWeaponContainerRect : %s"), *secondaryWeaponContainerRect.ToString());
}

void AFPPlayerController::OpenInventory()
{
	//오픈,클로즈 두개를 아예 별도로 써야되는 상황 떄문에 어쩔수 없이 openclose에서는 두번 체크함
	if (!inventory->IsInViewport())
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		isInventoryOpened = true;
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
		crosshair->SetVisibility(ESlateVisibility::Hidden);
		inventory->AddToViewport();
		itemDetailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFPPlayerController::CloseInventory()
{
	//오픈,클로즈 두개를 아예 별도로 써야되는 상황 떄문에 어쩔수 없이 openclose함수에서는 두번 체크함
	if (inventory->IsInViewport())
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		SetIgnoreLookInput(false);
		SetIgnoreMoveInput(false);
		isInventoryOpened = false;

		crosshair->SetVisibility(ESlateVisibility::Visible);
		inventory->RemoveFromViewport();
		itemDetailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFPPlayerController::OpenCloseInventory()
{
	//오픈,클로즈 두 개를 아예 별도로 써야되는 상황이 존재하여, 어쩔수 없이 openclose함수에서는 두번 체크함
	if (inventory->IsInViewport())
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

void AFPPlayerController::OpenItemDetailPanel(UItemIcon* pItemIcon)
{
	currentActiveItemIcon = pItemIcon;
	itemDetailPanel->SetVisibility(ESlateVisibility::Visible);

	FVector2D mouse;
	GetMousePosition(mouse.X, mouse.Y);
	float scale = UWidgetLayoutLibrary::GetViewportScale(this);
	mouse /= scale;
	itemDetailPanel->SetRenderTranslation(mouse);
}

void AFPPlayerController::AddItem(UItemInfo* itemInfo, UInventory* pInvenRef)
{
	if (inventory == nullptr || inventory->WidgetTree == nullptr || iconWidget == nullptr)
	{
		return;
	}

	UItemIcon* uiItem = inventory->WidgetTree->ConstructWidget<UItemIcon>(iconWidget); //TODO:WidgetTree가 계속 널이 되는 경우가 있는데 이 문제를 해결해야됨.
	UCanvasPanelSlot* panelSlotForItem = Cast<UCanvasPanelSlot>(itemContainer->AddChild(uiItem));
	uiItem->Slot = panelSlotForItem;
	uiItem->Init(itemInfo, pInvenRef, this);
	items.Add(uiItem);
}

void AFPPlayerController::DropItem(UItemIcon* pItemIcon)
{
	itemContainer->RemoveChild(pItemIcon);
	items.Remove(pItemIcon);
}

void AFPPlayerController::StartMoveItemPos(UItemInfo* pItemInfo)
{
	//컨트롤러가 캐릭터보다 먼저 초기화 되는 상황 때문에 다음과 같이 넣어놓음
	if(ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->StartMoveItemPos(pItemInfo);
	}
}

bool AFPPlayerController::CanItemMoveTo(FSlateRect pIntSlateRect)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		return ownerPlayerCharacter->CanItemMoveTo(pIntSlateRect);
	}
	else
	{
		return false;
	}
}

void AFPPlayerController::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	//RemoveAndAddItem
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->MoveItemTo(pItemInfo,pIntSlateRect);
	}
	else
	{
		return;
	}


	//UpdateUI
	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == pItemInfo)
		{
			FVector2D position = FVector2D(
				((int)pIntSlateRect.Left * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH), ((int)pIntSlateRect.Top * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT)
			);
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(position);
			break;
		}
	}

}

void AFPPlayerController::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->FailedToMoveItemPos(pItemInfo);
	}
}

void AFPPlayerController::AddPrimary(TSubclassOf<ABaseGun> pWeaponClass, UItemWeapon* pItemWeapon)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->AddPrimary(pWeaponClass, pItemWeapon);
	}

	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == (UItemInfo*)pItemWeapon) // 레퍼런스 찾기 용이니 임시직으로 변환해서 넣는것 TODO:만약에 예상과 결과가 다르면 int로 변환해서 넣는 방법도 고려해볼것
		{
			UE_LOG(LogTemp, Warning, TEXT("AddPrimary"));
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(primaryWeaponContainerSlot->GetPosition());
			itemContainer->RemoveChild(items[i]);
			items[i]->Slot = Cast<UCanvasPanelSlot>(primaryWeaponContainer->AddChild(items[i]));
			panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetSize(primaryWeaponContainerSlot->GetSize());
		}
	}
}

void AFPPlayerController::AddSecondary(TSubclassOf<ABaseGun> pWeaponClass, UItemWeapon* pItemWeapon)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->AddSecondary(pWeaponClass, pItemWeapon);
	}

	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == (UItemInfo*)pItemWeapon) // 레퍼런스 찾기 용이니 임시직으로 변환해서 넣는것 TODO:만약에 예상과 결과가 다르면 int로 변환해서 넣는 방법도 고려해볼것
		{
			UE_LOG(LogTemp, Warning, TEXT("AddSecondary"));
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(secondaryWeaponContainerSlot->GetPosition());
			itemContainer->RemoveChild(items[i]);
			items[i]->Slot = Cast<UCanvasPanelSlot>(secondaryWeaponContainer->AddChild(items[i]));
			panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetSize(secondaryWeaponContainerSlot->GetSize());
		}
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
	if (ownerPlayerCharacter == nullptr) 
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->RemovePrimary();
	}
}

void AFPPlayerController::RemoveSecondary()
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->RemoveSecondary();
	}
}

void AFPPlayerController::HealPlayer(float pHealAmount)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (ownerPlayerCharacter != nullptr)
	{
		float curHp = ownerPlayerCharacter->HealPlayer(pHealAmount);
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
	for (int i = items.Num() - 1; i >= 0; i--)
	{
		if (items[i] != nullptr && items[i]->IsValidLowLevel())
		{
			if(items[i]->itemInfo == nullptr ||
				!items[i]->itemInfo->IsValidLowLevel() ||
				items[i]->itemInfo->currentCapacity <= 0)
			{
				itemContainer->RemoveChild(items[i]);
				items.RemoveAt(i);
			}
		}
	}
}

void AFPPlayerController::Dead()
{
	youreDead->SetVisibility(ESlateVisibility::Visible);
	crosshair->SetVisibility(ESlateVisibility::Hidden);
	CloseInventory();
	if(gameMode)
	gameMode->PlayerDied();//메인화면으로 가는거 넣을것.
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

}

void AFPPlayerController::Exfiling()
{
	if (!isExfiling)
	{
		isExfiling = true;
		exfilCounter = 0.f;
		exfilAlert->SetVisibility(ESlateVisibility::Visible);
	}
}

void AFPPlayerController::ExfilingComplete()
{
	isExfiling = false;
	exfilAlert->SetVisibility(ESlateVisibility::Hidden);
	youveEscaped->SetVisibility(ESlateVisibility::Visible);
	if(gameMode)
		gameMode->ExfilCompleted();
}

void AFPPlayerController::CancelExfiling()
{
	if (isExfiling)
	{
		isExfiling = false;
		exfilAlert->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFPPlayerController::UseCurrentActiveItem()
{
	currentActiveItemIcon->UseItem();

}

void AFPPlayerController::DiscardCurrentActiveItem()
{
	currentActiveItemIcon->DropItem();
}

void AFPPlayerController::CloseAlert()
{
	alertHud->SetVisibility(ESlateVisibility::Hidden);
}
