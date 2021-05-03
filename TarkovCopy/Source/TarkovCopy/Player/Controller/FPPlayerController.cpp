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
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemHelmet.h"
#include <Blueprint/WidgetLayoutLibrary.h>

void AFPPlayerController::BeginPlay()
{
	bShowMouseCursor = false;

	healthHudUI = CreateWidget<UUserWidget>(this, healthHudWidget);
	healthHudUI->AddToViewport();
	healthHudBgUI = healthHudUI->GetWidgetFromName(TEXT("Splash"));
	flashHudBgUI = healthHudUI->GetWidgetFromName(TEXT("FlashSplash"));

	crosshairUI = CreateWidget<UUserWidget>(this, crosshairWidget);
	crosshairUI->AddToViewport();
	
	alertHudUI = CreateWidget<UUserWidget>(this, alertHudWidget);
	alertHudUI->AddToViewport();

	exfilAlertUI = CreateWidget<UUserWidget>(this, exfilAlertWidget);
	exfilAlertUI->AddToViewport();

	youreDeadUI = CreateWidget<UUserWidget>(this, youreDeadWidget);
	youreDeadUI->AddToViewport(2);

	youveEscapedUI = CreateWidget<UUserWidget>(this, youveEscapedWidget);
	youveEscapedUI->AddToViewport(2);

	hitIndicatorUI = CreateWidget<UUserWidget>(this, hitIndicatorWidget);
	hitIndicatorUI->AddToViewport();
	hitIndicatorActualUI = hitIndicatorUI->GetWidgetFromName(TEXT("HitIndicator"));


	alertHudUI->SetVisibility(ESlateVisibility::Hidden);
	exfilAlertUI->SetVisibility(ESlateVisibility::Hidden);
	youveEscapedUI->SetVisibility(ESlateVisibility::Hidden);
	youreDeadUI->SetVisibility(ESlateVisibility::Hidden);

	alertTypeText = Cast<UTextBlock>(alertHudUI->GetWidgetFromName(TEXT("AlertType")));
	missionObjectText = Cast<UTextBlock>(alertHudUI->GetWidgetFromName(TEXT("Object")));
	rangeText = Cast<UTextBlock>(alertHudUI->GetWidgetFromName(TEXT("Range")));
	exfilTimerText = Cast<UTextBlock>(exfilAlertUI->GetWidgetFromName(TEXT("ExfilTimer")));
	
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
		exfilTimerText->SetText(FText::FromString(FString::FormatAsNumber(timeToExfil - exfilCounter)));
		if ((timeToExfil - exfilCounter) <= 0.f)
		{
			ExfilingComplete();
		}
	}

	if (isFlashed)
	{
		flashTimer += DeltaTime;
		if (flashTimer > flashInterval / 2.f)
		{
			flashFadeAmount = ((flashInterval - flashTimer) / (flashInterval * 2.f));
			
			UE_LOG(LogTemp,Warning,TEXT("Fading : %f"), flashFadeAmount)

			flashHudBgUI->SetRenderOpacity(flashFadeAmount);
		}

		if (flashTimer > flashInterval)
		{
			isFlashed = false;
			flashTimer = 0.f;
			flashInterval = 0.f;
			flashHudBgUI->SetRenderOpacity(0.f);
		}
	}

	if (isHit)
	{
		hitIndicatorTimer += DeltaTime;

		//각도 적용

		FVector characterForward = ownerPlayerCharacter->GetActorForwardVector();
		FVector hitDir = (hitFromPos - ownerPlayerCharacter->GetActorLocation());
		float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(characterForward, hitDir) / (characterForward.Size() * hitDir.Size())));
		float crossAngle = FVector::DotProduct(FVector::CrossProduct(characterForward, hitDir),FVector::UpVector);

		UE_LOG(LogTemp, Warning, TEXT("Hit angle : %f , is left or right : %f"), angle, crossAngle);

		if (crossAngle < 0)
		{
			angle = -angle;
		}


		if (angle > 180.f)
		{
			angle -= 180.f;
		}
		hitIndicatorActualUI->SetRenderTransformAngle(angle);

		if (hitIndicatorTimer > hitIndicatorInterval / 2.f)
		{
			hitIndicatorFadeAmount = ((hitIndicatorInterval - hitIndicatorTimer) / (hitIndicatorInterval * 2.f));
			hitIndicatorActualUI->SetRenderOpacity(hitIndicatorFadeAmount);
		}

		if (hitIndicatorTimer > hitIndicatorInterval)
		{
			isHit = false;
			hitIndicatorTimer = 0.f;
			hitIndicatorActualUI->SetRenderOpacity(0.f);
		}
	}
}

void AFPPlayerController::InitInvenotry()
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (inventoryUI)
	{
		inventoryUI->RemoveFromViewport();
		inventoryUI = nullptr;
	}

	inventoryUI = CreateWidget<UUserWidget>(this, inventoryWidget);
	itemDetailPanel = Cast<UVerticalBox>(inventoryUI->GetWidgetFromName(TEXT("DetailPanel")));
	itemContainerUI = Cast<UCanvasPanel>(inventoryUI->GetWidgetFromName(TEXT("ItemContainer")));
	primaryWeaponContainerUI = Cast<UCanvasPanel>(inventoryUI->GetWidgetFromName(TEXT("PrimaryWeaponContainer")));
	secondaryWeaponContainerUI = Cast<UCanvasPanel>(inventoryUI->GetWidgetFromName(TEXT("SecondaryWeaponContainer")));
	helmetContainerUI = Cast<UCanvasPanel>(inventoryUI->GetWidgetFromName(TEXT("HelmetContainer")));
	
	itemContainerUISlot = Cast<UCanvasPanelSlot>(itemContainerUI->Slot);
	primaryWeaponContainerUISlot = Cast<UCanvasPanelSlot>(primaryWeaponContainerUI->Slot);
	secondaryWeaponContainerUISlot = Cast<UCanvasPanelSlot>(secondaryWeaponContainerUI->Slot);
	helmetContainerUISlot = Cast<UCanvasPanelSlot>(helmetContainerUI->Slot);
	
	itemContainerUISlot->SetSize(FVector2D(UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT) * ownerPlayerCharacter->inventory->GetBackpack()->GetBackpackSize());

	itemContainerRect.Left = itemContainerUISlot->GetPosition().X;
	itemContainerRect.Top = itemContainerUISlot->GetPosition().Y;
	itemContainerRect.Right = itemContainerRect.Left + itemContainerUISlot->GetSize().X;
	itemContainerRect.Bottom = itemContainerRect.Top + itemContainerUISlot->GetSize().Y;

	primaryWeaponContainerRect.Left = primaryWeaponContainerUISlot->GetPosition().X;
	primaryWeaponContainerRect.Top = primaryWeaponContainerUISlot->GetPosition().Y;
	primaryWeaponContainerRect.Right = primaryWeaponContainerRect.Left + primaryWeaponContainerUISlot->GetSize().X;
	primaryWeaponContainerRect.Bottom = primaryWeaponContainerRect.Top + primaryWeaponContainerUISlot->GetSize().Y;

	secondaryWeaponContainerRect.Left = secondaryWeaponContainerUISlot->GetPosition().X;
	secondaryWeaponContainerRect.Top = secondaryWeaponContainerUISlot->GetPosition().Y;
	secondaryWeaponContainerRect.Right = secondaryWeaponContainerRect.Left + secondaryWeaponContainerUISlot->GetSize().X;
	secondaryWeaponContainerRect.Bottom = secondaryWeaponContainerRect.Top + secondaryWeaponContainerUISlot->GetSize().Y;

	helmetContainerRect.Left = helmetContainerUISlot->GetPosition().X;
	helmetContainerRect.Top = helmetContainerUISlot->GetPosition().Y;
	helmetContainerRect.Right = helmetContainerRect.Left + helmetContainerUISlot->GetSize().X;
	helmetContainerRect.Bottom = helmetContainerRect.Top + helmetContainerUISlot->GetSize().Y;

}

void AFPPlayerController::OpenInventory()
{
	//오픈,클로즈 두개를 아예 별도로 써야되는 상황 떄문에 어쩔수 없이 openclose에서는 두번 체크함
	if (!inventoryUI->IsInViewport())
	{
		LockOpenUI();
		isInventoryOpened = true;
		crosshairUI->SetVisibility(ESlateVisibility::Hidden);
		inventoryUI->AddToViewport();
		itemDetailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFPPlayerController::CloseInventory()
{
	//오픈,클로즈 두개를 아예 별도로 써야되는 상황 떄문에 어쩔수 없이 openclose함수에서는 두번 체크함
	if (inventoryUI->IsInViewport())
	{
		UnlockCloseUI();
		isInventoryOpened = false;
		crosshairUI->SetVisibility(ESlateVisibility::Visible);
		inventoryUI->RemoveFromViewport();
		itemDetailPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFPPlayerController::OpenCloseInventory()
{
	//오픈,클로즈 두 개를 아예 별도로 써야되는 상황이 존재하여, 어쩔수 없이 openclose함수에서는 두번 체크함
	if (inventoryUI->IsInViewport())
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

void AFPPlayerController::LockOpenUI()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
}

void AFPPlayerController::UnlockCloseUI()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);

	UE_LOG(LogTemp,Warning,TEXT("Bang bang"))
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
	if (inventoryUI == nullptr || inventoryUI->WidgetTree == nullptr || iconWidget == nullptr)
	{
		return;
	}

	UItemIcon* uiItem = inventoryUI->WidgetTree->ConstructWidget<UItemIcon>(iconWidget); //TODO:WidgetTree가 계속 널이 되는 경우가 있는데 이 문제를 해결해야됨.
	UCanvasPanelSlot* panelSlotForItem = Cast<UCanvasPanelSlot>(itemContainerUI->AddChild(uiItem));
	uiItem->Slot = panelSlotForItem;
	uiItem->Init(itemInfo, pInvenRef, this);
	items.Add(uiItem);
}

//TODO:RemoveItemUI로 변경
void AFPPlayerController::DropItem(UItemIcon* pItemIcon)
{
	itemContainerUI->RemoveChild(pItemIcon);
	//
	if (!ownerPlayerCharacter->inventory->HasItem(pItemIcon->itemInfo))
	{
		primaryWeaponContainerUI->RemoveChild(pItemIcon);
		secondaryWeaponContainerUI->RemoveChild(pItemIcon);
		helmetContainerUI->RemoveChild(pItemIcon);
	}
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
			
			if (itemContainerUI->HasChild(items[i]))
			{
				UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
				panelSlot->SetPosition(position);
			}
			else
			{
				if (primaryWeaponContainerUI->HasChild(items[i]))
					primaryWeaponContainerUI->RemoveChild(items[i]);

				if (secondaryWeaponContainerUI->HasChild(items[i]))
					secondaryWeaponContainerUI->RemoveChild(items[i]);

				items[i]->Slot = Cast<UCanvasPanelSlot>(itemContainerUI->AddChild(items[i]));

				UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
				panelSlot->SetPosition(position);
				panelSlot->SetSize(FVector2D(items[i]->itemInfo->width * UMGPublicProperites::BASIC_INVENTORY_GRID_WIDTH, items[i]->itemInfo->height * UMGPublicProperites::BASIC_INVENTORY_GRID_HEIGHT));
			}

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

bool AFPPlayerController::IsInItemContainer(UItemIcon* pItemInfo) const
{
	FVector2D finalPosition = pItemInfo->GetCachedGeometry().GetAbsolutePosition();
	if (itemContainerRect.ContainsPoint(finalPosition))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AFPPlayerController::IsInEquipmentSlot(UItemIcon* pItemInfo) const
{
	//가로만 조정한다
	FVector2D finalPosition = pItemInfo->GetCachedGeometry().GetAbsolutePosition();
	finalPosition.X += pItemInfo->GetCachedGeometry().GetAbsoluteSize().X / 2.f;
	finalPosition.Y += pItemInfo->GetCachedGeometry().GetAbsoluteSize().Y / 2.f;
	if (primaryWeaponContainerRect.ContainsPoint(finalPosition) ||
		secondaryWeaponContainerRect.ContainsPoint(finalPosition)||
		helmetContainerRect.ContainsPoint(finalPosition))
	{
		return true;
	}
	else
	{
		return false;
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

	//AddPrimary UI 업데이트 -> 위치를 itemInfo 기반으로 옮기지 않는 별도 예외
	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == (UItemInfo*)pItemWeapon) // 레퍼런스 찾기 용이니 임시직으로 변환해서 넣는것 TODO:만약에 예상과 결과가 다르면 int로 변환해서 넣는 방법도 고려해볼것
		{
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(primaryWeaponContainerUISlot->GetPosition());

			itemContainerUI->RemoveChild(items[i]);
			items[i]->Slot = Cast<UCanvasPanelSlot>(primaryWeaponContainerUI->AddChild(items[i]));
			panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);

			panelSlot->SetSize(primaryWeaponContainerUISlot->GetSize());
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

	//AddSecondary UI 업데이트 -> 위치를 itemInfo 기반으로 옮기지 않는 별도 예외
	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == (UItemInfo*)pItemWeapon) // 레퍼런스 찾기 용이니 임시직으로 변환해서 넣는것 TODO:만약에 예상과 결과가 다르면 int로 변환해서 넣는 방법도 고려해볼것
		{
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(secondaryWeaponContainerUISlot->GetPosition());

			itemContainerUI->RemoveChild(items[i]);
			items[i]->Slot = Cast<UCanvasPanelSlot>(secondaryWeaponContainerUI->AddChild(items[i]));

			panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetSize(secondaryWeaponContainerUISlot->GetSize());
		}
	}
}

void AFPPlayerController::SetADS()
{
	crosshairUI->SetVisibility(ESlateVisibility::Hidden);
}

void AFPPlayerController::SetHipfire()
{
	crosshairUI->SetVisibility(ESlateVisibility::Visible);
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

void AFPPlayerController::AddHelmet(UItemHelmet* pHelmetInfo)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->AddHelmet(pHelmetInfo);
	}

	//AddPrimary UI 업데이트 -> 위치를 itemInfo 기반으로 옮기지 않는 별도 예외
	for (int i = 0; i < items.Num(); i++)
	{
		if (items[i]->itemInfo == (UItemInfo*)pHelmetInfo) // 레퍼런스 찾기 용이니 임시직으로 변환해서 넣는것 TODO:만약에 예상과 결과가 다르면 int로 변환해서 넣는 방법도 고려해볼것
		{
			UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);
			panelSlot->SetPosition(helmetContainerUISlot->GetPosition());

			itemContainerUI->RemoveChild(items[i]);
			items[i]->Slot = Cast<UCanvasPanelSlot>(helmetContainerUI->AddChild(items[i]));
			panelSlot = Cast<UCanvasPanelSlot>(items[i]->Slot);

			panelSlot->SetSize(helmetContainerUISlot->GetSize());
		}
	}
}

void AFPPlayerController::RemoveHelmet(UItemHelmet* pHelmetInfo)
{
	if (ownerPlayerCharacter == nullptr)
		ownerPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (ownerPlayerCharacter != nullptr)
	{
		ownerPlayerCharacter->RemoveHelmet(pHelmetInfo);
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
	healthHudBgUI->SetRenderOpacity(opacityAmount);
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
				itemContainerUI->RemoveChild(items[i]);
				items.RemoveAt(i);
			}
		}
	}
}

void AFPPlayerController::Dead()
{
	youreDeadUI->SetVisibility(ESlateVisibility::Visible);
	crosshairUI->RemoveFromViewport();
	hitIndicatorUI->RemoveFromViewport();
	CloseInventory();
	if(gameMode)
	gameMode->PlayerDied();//메인화면으로 가는거 넣을것.
}

void AFPPlayerController::GetFlashed(float pFlashTime, FVector pFlashbangPos)
{
	FVector dirToFlashbang = pFlashbangPos - ownerPlayerCharacter->GetActorLocation();
	FVector playerForward = ownerPlayerCharacter->GetActorForwardVector();

	float vectorResult = FVector::DotProduct(dirToFlashbang, playerForward) / dirToFlashbang.Size() * playerForward.Size();
	
	//TODO: 섬광탄에 영향받는 시야각을 나중에 CONSTANT로 몰아서 정의할 것.
	//TODO: 0.6이면 COS하면 50도 가까이 됨 - 각도로 계산하면 각도로 바꿔주는 비용이 발생하기 때문에, 그냥 COS() 값으로 계산.
	UE_LOG(LogTemp, Warning, TEXT("GET FLASHED IN DOT PRODUCT : %f"), FVector::DotProduct(dirToFlashbang, playerForward));
	UE_LOG(LogTemp, Warning, TEXT("GET FLASHED IN COS : %f"), vectorResult);
	UE_LOG(LogTemp, Warning, TEXT("GET FLASHED IN ANGLE : %f"), (FMath::RadiansToDegrees(FMath::Acos(vectorResult))));
	if (vectorResult > 0.6f)
	{
		UE_LOG(LogTemp, Warning, TEXT("GET FLASHED"));
		flashInterval = pFlashTime;
		flashTimer = 0.f;
		isFlashed = true;
		//TODO: 섬광탄 스크린 효과
		flashHudBgUI->SetRenderOpacity(1.f);
	}



}

void AFPPlayerController::ShowHitIndicator(FVector pHitFrom)
{
	hitFromPos = pHitFrom;
	isHit = true;
	hitIndicatorActualUI->SetRenderOpacity(1.f);

}

void AFPPlayerController::ShowQuestInfo(FString itemName, float distance)
{
	alertHudUI->SetVisibility(ESlateVisibility::Visible);
	alertTypeText->SetText(FText::FromString("Quest"));

	FString objectMessage = "Get the ";
	objectMessage.Append(itemName);
	missionObjectText->SetText(FText::FromString(objectMessage));

	FString rangeString = FString::FormatAsNumber((int)distance);
	rangeString.Append("m");
	rangeText->SetText(FText::FromString(rangeString));

	FTimerHandle timers;
	GetWorldTimerManager().SetTimer(timers,this,&AFPPlayerController::CloseAlert, 3.f, false);
}

void AFPPlayerController::ShowExfilPoints(FString exfilPointsName, float distance)
{
	alertHudUI->SetVisibility(ESlateVisibility::Visible);

	alertTypeText->SetText(FText::FromString("Exfil"));

	FString objectMessage = "Get to the ";
	objectMessage.Append(exfilPointsName);
	missionObjectText->SetText(FText::FromString(objectMessage));

	FString rangeString = FString::FormatAsNumber((int)distance);
	rangeString.Append("m");
	rangeText->SetText(FText::FromString(rangeString));

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
		exfilAlertUI->SetVisibility(ESlateVisibility::Visible);
	}
}

void AFPPlayerController::ExfilingComplete()
{
	isExfiling = false;
	exfilAlertUI->SetVisibility(ESlateVisibility::Hidden);
	youveEscapedUI->SetVisibility(ESlateVisibility::Visible);
	if(gameMode)
		gameMode->ExfilCompleted();
}

void AFPPlayerController::CancelExfiling()
{
	if (isExfiling)
	{
		isExfiling = false;
		exfilAlertUI->SetVisibility(ESlateVisibility::Hidden);
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
	alertHudUI->SetVisibility(ESlateVisibility::Hidden);
}
