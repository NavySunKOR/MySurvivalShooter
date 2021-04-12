// Copyright Epic Games, Inc. All Rights Reserved.

#include "TarkovCopyGameModeBase.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include <TimerManager.h>
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>


ATarkovCopyGameModeBase::ATarkovCopyGameModeBase()
{
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
	InitalizeAI();
}

void ATarkovCopyGameModeBase::InitalizeAI()
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
}

void ATarkovCopyGameModeBase :: QuestCompleted(AInteractableObject* questItem)
{

}

void ATarkovCopyGameModeBase::ExfilCompleted()
{
	isPlayerEscaped = true;
	UGameplayStatics::PlaySound2D(GetWorld(), escapedSound);
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATarkovCopyGameModeBase::ReturnToMainMenu, 4.f);
}

void ATarkovCopyGameModeBase::TryExfil()
{
}

void ATarkovCopyGameModeBase::CancelExfil()
{

}
