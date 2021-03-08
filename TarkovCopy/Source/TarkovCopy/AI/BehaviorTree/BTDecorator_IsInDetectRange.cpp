// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInDetectRange.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

bool UBTDecorator_IsInDetectRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//Super::WrappedCanExecute(OwnerComp, NodeMemory);

	AAICharacter* owner = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (owner == nullptr)
	{
		return false;
	}



	if (!owner->outIsPlayerDetected)
	{
		return false;
	}
	else
	{
		return true;
	}
}