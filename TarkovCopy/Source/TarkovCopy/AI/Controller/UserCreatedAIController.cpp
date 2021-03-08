// Fill out your copyright notice in the Description page of Project Settings.

#include "UserCreatedAIController.h"
#include <Components/SphereComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "TarkovCopy/AI/Character/AICharacter.h"



void AUserCreatedAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(blackboardTree);
	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	aiCharacter = Cast<AAICharacter>(GetPawn());
}

void AUserCreatedAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (aiCharacter && aiCharacter->outIsPlayerDetected)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastPlayerLocation"), aiCharacter->outPlayerLocation);
	}
}
