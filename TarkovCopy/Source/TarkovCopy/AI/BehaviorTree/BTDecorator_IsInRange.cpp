// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/AI/Character/AICharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "BTDecorator_IsInRange.h"


bool UBTDecorator_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	AAICharacter* owner = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (owner == nullptr)
	{
		return false;
	}

	if (!owner->outIsPlayerDetected)
		return false;
	else
	{
		if (OwnerComp.GetAIOwner()->LineOfSightTo(owner->trackingTarget))
		{
			owner->targetActor = owner->trackingTarget;
			return true;
		}
		else
		{
			owner->targetActor = nullptr;
			return false;
		}
	}
}
