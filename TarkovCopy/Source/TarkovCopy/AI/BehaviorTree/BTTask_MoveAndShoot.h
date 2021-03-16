// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAndShoot.generated.h"

/**
 * 
 */
class AAICharacter;
class ATarkovCopyGameModeBase;
UCLASS()
class TARKOVCOPY_API UBTTask_MoveAndShoot : public UBTTaskNode
{
	GENERATED_BODY()
private:
	FVector playerLocation;
	AAICharacter* aiCharacter;
	ATarkovCopyGameModeBase* gameMode;
protected:
	bool IsInRange(UBehaviorTreeComponent& OwnerComp) const;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
