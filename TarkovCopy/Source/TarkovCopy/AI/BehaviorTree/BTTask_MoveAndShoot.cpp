// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveAndShoot.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

bool UBTTask_MoveAndShoot::IsInRange(UBehaviorTreeComponent& OwnerComp) const
{
	return OwnerComp.GetAIOwner()->LineOfSightTo(aiCharacter->trackingTarget) &&
		(aiCharacter->trackingTarget->GetActorLocation() - aiCharacter->GetActorLocation()).Size() < aiCharacter->GetCurrentWeaponRange();
}

EBTNodeResult::Type UBTTask_MoveAndShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (aiCharacter != Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		aiCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	}

	if(gameMode == nullptr)
		gameMode = Cast<ATarkovCopyGameModeBase>(GetWorld()->GetAuthGameMode());

	if (aiCharacter == nullptr || (gameMode && (gameMode->isPlayerEscaped || gameMode->isPlayerDied)))
	{
		return EBTNodeResult::Failed;
	}


	if (!aiCharacter->isFlashed)
	{
		if (IsInRange(OwnerComp))
		{
			aiCharacter->targetActor = aiCharacter->trackingTarget;
			aiCharacter->FireWeapon();
			OwnerComp.GetAIOwner()->StopMovement();
		}
		else
		{
			if (aiCharacter->targetActor != nullptr)
				OwnerComp.GetAIOwner()->MoveToLocation(aiCharacter->targetActor->GetActorLocation());
			else
				OwnerComp.GetAIOwner()->MoveToLocation(aiCharacter->trackingTarget->GetActorLocation());
		}
	}

	return EBTNodeResult::Succeeded;
}
