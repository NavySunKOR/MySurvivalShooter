// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInRange.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>


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
	/*	FVector targetDir = (owner->trackingTarget->GetActorLocation() - owner->GetActorLocation());
		targetDir.Normalize();
		float angleCos = FVector::DotProduct(owner->GetActorForwardVector(), targetDir) / owner->GetActorForwardVector().Size() * targetDir.Size();
		float toAngle = FMath::RadiansToDegrees(FMath::Acos(angleCos));*/

		if (OwnerComp.GetAIOwner()->LineOfSightTo(owner->trackingTarget) && 
			(owner->trackingTarget->GetActorLocation() - owner->GetActorLocation()).Size() < owner->GetCurrentWeaponRange())
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
