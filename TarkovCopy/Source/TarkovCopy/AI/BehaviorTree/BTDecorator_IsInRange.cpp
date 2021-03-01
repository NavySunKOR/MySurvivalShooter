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

	FVector startPos = owner->GetActorLocation();
	FVector dir = owner->outPlayerLocation -  owner->GetActorLocation();
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(owner);

	if (!owner->outIsPlayerDetected)
		return false;
	else
	{
		if (GetWorld()->LineTraceSingleByChannel(hit, startPos, dir * 500.f, ECollisionChannel::ECC_Pawn, CollisionParams))
		{
			if (hit.Actor->ActorHasTag(TEXT("Player")))
			{
				owner->targetActor = hit.Actor.Get();
				return true;
			}
			else
			{
				owner->targetActor = nullptr;
				return false;
			}
		}
		else
		{
			owner->targetActor = nullptr;
			return false;
		}
	}
}
