// Copyright Epic Games, Inc. All Rights Reserved.

#include "TarkovCopyGameModeBase.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Interactable/InteractableObject.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Utils/JsonSaveAndLoader.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <TimerManager.h>
#include <EngineUtils.h>


ATarkovCopyGameModeBase::ATarkovCopyGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<AAICharacter> AICharacter(TEXT("Blueprint'/Game/Blueprints/AI/BP_AICharacter.BP_AICharacter_C'"));
	if (AICharacter.Succeeded() && AICharacter.Class != NULL)
		aiCharacter = AICharacter.Class;
}

void ATarkovCopyGameModeBase::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void ATarkovCopyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<AFPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InitializeAI();
}

void ATarkovCopyGameModeBase::InitializeAI()
{
	TArray<AActor*> allSpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("SpawnPoints"), allSpawnPoints);

	for (AActor* spawnPoint : allSpawnPoints)
	{
		UWorld* gatcha = GetWorld();
		AAICharacter* character = gatcha->SpawnActor<AAICharacter>(aiCharacter); 
		character->SetActorLocation(spawnPoint->GetActorLocation());
		aiPlayers.Add(character);
	}
}

void ATarkovCopyGameModeBase::PlayerDied()
{
	isPlayerDied = true;
	UGameplayStatics::PlaySound2D(GetWorld(), diedSound);
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer,this, &ATarkovCopyGameModeBase::ReturnToMainMenu,4.f);
	JsonSaveAndLoader::SaveBackpackItemContainers(TArray<UItemInfo*>());
}

void ATarkovCopyGameModeBase :: QuestCompleted(AInteractableObject* questItem)
{

}

void ATarkovCopyGameModeBase::ExfilCompleted()
{
	isPlayerEscaped = true;
	UGameplayStatics::PlaySound2D(GetWorld(), escapedSound);
	FTimerHandle timer;
	JsonSaveAndLoader::SaveBackpackItemContainers(playerController->GetItemContainers());
	playerController->SaveEquipments();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATarkovCopyGameModeBase::ReturnToMainMenu, 4.f);

}

void ATarkovCopyGameModeBase::TryExfil()
{
}

void ATarkovCopyGameModeBase::CancelExfil()
{

}