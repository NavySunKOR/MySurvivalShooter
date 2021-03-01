// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UserCreatedAIController.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API AUserCreatedAIController : public AAIController
{
	GENERATED_BODY()


protected:
	APawn* playerPawn;
	virtual void BeginPlay() override;
	/*virtual void Tick(float DeltaTime) override;*/

public:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* blackboardTree;




};