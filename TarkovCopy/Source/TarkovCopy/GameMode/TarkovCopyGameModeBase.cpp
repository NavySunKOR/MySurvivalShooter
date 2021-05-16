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
void ATarkovCopyGameModeBase::OpenPauseMenu()
{
	isPauseMenuOpened = true;
	playerController->UnlockCloseUI();
	playerController->LockOpenUI();
	pauseMenuWidget->AddToViewport();
}
void ATarkovCopyGameModeBase::ClosePauseMenu()
{
	isPauseMenuOpened = false;
	playerController->UnlockCloseUI();
	pauseMenuWidget->RemoveFromViewport();
}
void ATarkovCopyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<AFPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InitializeAI();
	InitializeSystemUI();
}

void ATarkovCopyGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (playerController == nullptr)
	{
		playerController = Cast<AFPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	if (playerController->WasInputKeyJustPressed(EKeys::Tab))
	{
		if (optionMenuWidget->IsInViewport())
		{
			CloseOptionMenu();
			ClosePauseMenu();
		}
		else
		{
			if (!pauseMenuWidget->IsInViewport())
			{
				//메뉴 열기
				OpenPauseMenu();
			}
			else
			{
				//메뉴 닫기
				ClosePauseMenu();
			}
		}

	}
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

void ATarkovCopyGameModeBase::InitializeSystemUI()
{
	pauseMenuWidget = CreateWidget<UUserWidget>(playerController, pauseMenuWidgetOrigin);
	optionMenuWidget = CreateWidget<UUserWidget>(playerController, optionMenuWidgetOrigin);
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

//BlueprintCallables

void ATarkovCopyGameModeBase::ResumeGame()
{
	ClosePauseMenu();
}

void ATarkovCopyGameModeBase::QuitGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void ATarkovCopyGameModeBase::OpenOptionMenu()
{
	ClosePauseMenu(); 

	isPauseMenuOpened = true;
	playerController->LockOpenUI();
	optionMenuWidget->AddToViewport();
}

void ATarkovCopyGameModeBase::CloseOptionMenu()
{
	isPauseMenuOpened = false;
	optionMenuWidget->RemoveFromViewport();
	OpenPauseMenu();
}
