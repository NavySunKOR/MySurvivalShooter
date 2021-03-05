// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "BTTask_FireAtEnemy.h"


EBTNodeResult::Type UBTTask_FireAtEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* aiCharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	ATarkovCopyGameModeBase* gameMode = Cast<ATarkovCopyGameModeBase>(GetWorld()->GetAuthGameMode());

	if (aiCharacter == nullptr || (gameMode && (gameMode->isPlayerEscaped || gameMode->isPlayerDied)))
	{
		return EBTNodeResult::Failed;
	}
	aiCharacter->FireWeapon();

	return EBTNodeResult::Succeeded;
}