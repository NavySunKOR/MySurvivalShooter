// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/SphereComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Kismet/GameplayStatics.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "UserCreatedAIController.h"



void AUserCreatedAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(blackboardTree);
	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("Player : %s"), *playerPawn->GetActorLocation().ToCompactString());
	
}

//void AUserCreatedAIController::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	UE_LOG(LogTemp, Warning, TEXT("Player : %s"), *playerPawn->GetActorLocation().ToCompactString());
//	GetBlackboardComponent()->SetValueAsVector(TEXT("LastPlayerLocation"), playerPawn->GetActorLocation());
//}
