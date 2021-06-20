// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerComponent.h"
#include "PlayerMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TARKOVCOPY_API UPlayerMovementComponent : public UBasePlayerComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerMovementComponent();

private :

	float MaxWalkValue = 0.f;
	float WalkingSpeed = 0.f;
	float AdsWalkingSpeed = 0.f;
	float SprintSpeed = 0.f;

	void CheckCloseToWall();
	void MovementCheck();

public:
	bool IsCloseToWall = false;
	bool IsCrouch = false;
	bool IsSprinting = false;
	float MoveVerticalValue = 0.f;
	float MoveHorizontalValue = 0.f;

	void Init(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter,float pSprintSpeed, float pWalkingSpeed, float pAdsWalkingSpeed);
	virtual void Loop() override;
	virtual void End() override;

	//SetValueFromMovement
	void SetSprinting();
	void SetWalking();
	void SetCrouch();
	void SetUncrouch();

};
