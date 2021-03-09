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

	UE_LOG(LogTemp, Warning, TEXT("TarkovCopyGameModeBegin"))
		InitalizeAI();
}

void ATarkovCopyGameModeBase::InitalizeAI()
{
	TArray<AActor*> allSpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("SpawnPoints"), allSpawnPoints);

	for (AActor* spawnPoint : allSpawnPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnPoint"))
		UE_LOG(LogTemp, Warning, TEXT("Hoxy....? : %d"), GetWorld())
		UWorld* gatcha = GetWorld();

		UE_LOG(LogTemp, Warning, TEXT("Gacha sanai %d"), aiCharacter);
		AAICharacter* character = gatcha->SpawnActor<AAICharacter>(aiCharacter); // 근본없는 언리얼 새끼는 여기서 패키지로 나가면 터진다 ㅋㅋ

		UE_LOG(LogTemp, Warning, TEXT("Spawned?"))
		character->SetActorLocation(spawnPoint->GetActorLocation());

		UE_LOG(LogTemp, Warning, TEXT("yes?"))
		aiPlayers.Add(character);
	}
}

void ATarkovCopyGameModeBase::PlayerDied()
{
	isPlayerDied = true;
	UE_LOG(LogTemp, Warning, TEXT("Died bgm"));
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
	UE_LOG(LogTemp, Warning, TEXT("Exfil bgm"));
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
