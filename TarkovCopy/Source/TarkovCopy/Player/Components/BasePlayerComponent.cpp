// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerComponent.h"

// Sets default values for this component's properties
UBasePlayerComponent::UBasePlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBasePlayerComponent::Init(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
{
	PlayerController = pPlayerController;
	PlayerCharacter = pPlayerCharacter;
}

void UBasePlayerComponent::Loop()
{

}

void UBasePlayerComponent::End()
{
}

