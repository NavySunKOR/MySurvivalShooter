// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/GameSession.h>
#include <Runtime/Engine/Public/EngineUtils.h>
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/UI/InGameHUD.h"
#include "TarkovCopy/Interactable/QuestItem.h"
#include <Engine/TriggerBox.h>


void AEscapeGameMode::StartPlay()
{
	Super::StartPlay();
}

void AEscapeGameMode::PostLogin(APlayerController* pPlayerCon)
{
	Super::PostLogin(pPlayerCon);
}

void AEscapeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetMatchState() == MatchState::InProgress)
	{
		if (!isInitializedComplete)
		{
			playerCon = Cast<AFPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			SelectQuestItems();
			isInitializedComplete = true;
		}
	}
}

void AEscapeGameMode::SelectQuestItems()
{
	TActorRange<AQuestItem> items = TActorRange<AQuestItem>(GetWorld());
	for (AQuestItem* item : items)
	{
		item->SetActorHiddenInGame(true);
		allQuestItems.Add(item);
	}
	
	int num = FMath::RandRange(0, allQuestItems.Num() - 1);

	activeQuestItem = allQuestItems[num];
	activeQuestItem->SetActorHiddenInGame(false);

	AInGameHUD* ingameHud = Cast<AInGameHUD>(playerCon->GetHUD());
	if (ingameHud)
	{
		ingameHud->ShowQuestInfo(activeQuestItem->GetName(), (activeQuestItem->GetActorLocation() - playerCon->GetPawn()->GetActorLocation()).Size() / 100.f);
	}
}
void AEscapeGameMode::SelectExfilPoint()
{

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("ExfilPoint"), allExfilPoints);
	for (AActor* exfil : allExfilPoints)
	{
		exfil->SetActorHiddenInGame(true);
	}

	int num = FMath::RandRange(0, allExfilPoints.Num() - 1);

	activeExfilPoint = allExfilPoints[num];
	activeExfilPoint->SetActorHiddenInGame(false);

	if (playerCon != nullptr)
	{
		AInGameHUD* ingameHud = Cast<AInGameHUD>(playerCon->GetHUD());
		if (ingameHud)
		{
			ingameHud->ShowExfilPoints(activeExfilPoint->GetName(), (activeExfilPoint->GetActorLocation() - playerCon->GetPawn()->GetActorLocation()).Size() / 100.f);
		}
	}
}

void AEscapeGameMode::QuestCompleted(AInteractableObject* questItem)
{
	if (activeQuestItem == questItem)
	{
		isQuestCompleted = true;
		activeQuestItem->SetActorHiddenInGame(true);
		SelectExfilPoint();
	}
}
void AEscapeGameMode::TryExfil()
{
	if (isQuestCompleted)
	{
		//Exfil
		playerCon->Exfiling();
	}
	else
	{
		playerCon->ShowCannotExfil();
	}
}

void AEscapeGameMode::CancelExfil()
{
	playerCon->CancelExfiling();
}