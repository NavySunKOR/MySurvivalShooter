// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Weapons/BaseGun.h"

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerMovementComponent::InitMovementComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter,float pSprintSpeed, float pWalkingSpeed, float pAdsWalkingSpeed)
{
	Super::Init(pPlayerController, pPlayerCharacter);
	SprintSpeed = pSprintSpeed;
	WalkingSpeed = pWalkingSpeed;
	AdsWalkingSpeed = pAdsWalkingSpeed;

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void UPlayerMovementComponent::Loop()
{
	CheckCloseToWall();
	MovementCheck();
}

void UPlayerMovementComponent::End()
{

}

void UPlayerMovementComponent::CheckCloseToWall()
{
	FVector startPos;
	FVector dir;
	FHitResult hit;
	startPos = PlayerCharacter->GetActorLocation();
	dir = PlayerCharacter->GetActorForwardVector();
	FCollisionQueryParams paramCol;
	paramCol.AddIgnoredActor(PlayerCharacter);
	if (PlayerCharacter->GetCurrentActiveGun())
		paramCol.AddIgnoredActor(PlayerCharacter->GetCurrentActiveGun());

	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, startPos + dir * 150.f, ECollisionChannel::ECC_Pawn, paramCol))
	{
		IsCloseToWall = true;
	}
	else
	{
		IsCloseToWall = false;
	}
}

void UPlayerMovementComponent::MovementCheck()
{
	if (MoveVerticalValue * MoveVerticalValue > 0 || MoveHorizontalValue * MoveHorizontalValue > 0)
	{
		//Can sprint?
		if (MoveVerticalValue > 0 && IsSprinting)
		{
			MaxWalkValue = SprintSpeed;
		}
		else
		{
			MaxWalkValue = WalkingSpeed;
		}

		//IsAds?
		MaxWalkValue = (PlayerCharacter->IsAds()) ? AdsWalkingSpeed : MaxWalkValue;


		//Final Movement
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkValue;
		PlayerCharacter->AddMovementInput((PlayerCharacter->GetActorForwardVector() * MoveVerticalValue + PlayerCharacter->GetActorRightVector() * MoveHorizontalValue) / 1.4f);
	}
}

void UPlayerMovementComponent::SetCrouch()
{
	if (IsCrouch)
	{
		PlayerCharacter->UnCrouch();
		IsCrouch = false;
	}
}

void UPlayerMovementComponent::SetUncrouch()
{
	if (IsSprinting)
	{
		IsSprinting = false;
	}
	PlayerCharacter->Crouch();
	IsCrouch = true;
}

void UPlayerMovementComponent::SetSprinting()
{
	if (
		!IsSprinting
		&&
		!IsCrouch 
		&& 
		((MoveVerticalValue > 0 && MoveHorizontalValue == 0) || (MoveVerticalValue > 0 && MoveHorizontalValue * MoveHorizontalValue > 0)))
	{
		IsSprinting = true;
	}
}

void UPlayerMovementComponent::SetWalking()
{
	IsSprinting = false;
}