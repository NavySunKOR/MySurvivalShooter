// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SearchEnemy.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

EBTNodeResult::Type UBTTask_SearchEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* owner = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (owner == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (owner->outIsPlayerDetected)
	{
		blackboard->SetValueAsVector(TEXT("LastPlayerLocation"), owner->outPlayerLocation);
		blackboard->SetValueAsBool(TEXT("IsPlayerContacted"), owner->outIsPlayerDetected);
	}

	return EBTNodeResult::Succeeded;
}
