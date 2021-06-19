// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"

class AFPPlayerController;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TARKOVCOPY_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerMovementComponent();
	FORCEINLINE void SetPlayerInformation(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
	{
		PlayerController = pPlayerController;
		PlayerCharacter = pPlayerCharacter;
	}

private :
	AFPPlayerController* PlayerController;
	APlayerCharacter* PlayerCharacter;
	FTimerHandle LoopTimer;
	UPROPERTY(EditAnywhere, Category = "MovementComponent")
	float LoopFrame = 60.f;
	float MaxWalkValue = 0.f;
	

	float WalkingSpeed = 0.f;
	float AdsWalkingSpeed = 0.f;
	float SprintSpeed = 0.f;

public:
	bool IsCloseToWall = false;
	bool IsCrouch = false;
	bool IsSprinting = false;
	float MoveVerticalValue = 0.f;
	float MoveHorizontalValue = 0.f;

	void Init(float pSprintSpeed, float pWalkingSpeed, float pAdsWalkingSpeed);
	void Loop();
	void End();

	//SetValueFromMovement
	void SetSprinting();
	void SetWalking();
	void SetCrouch();
	void SetUncrouch();


protected:

	void CheckCloseToWall();
	void MovementCheck();
};
