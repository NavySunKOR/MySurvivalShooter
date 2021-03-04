// Fill out your copyright notice in the Description page of Project Settings.


#include "OpendoorComponent.h"

void UOpendoorComponent::BeginPlay()
{
	Super::BeginPlay();
	myOwnComponent = GetOwner();
	originRotation = myOwnComponent->GetActorQuat();
	targetRotation = FQuat(originRotation.Rotator() + FRotator(0, 0, 90.f));
}

void UOpendoorComponent::Interact()
{
	isDoorOpened = !isDoorOpened;
}

void UOpendoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isDoorOpened)
	{
		myOwnComponent->SetActorRelativeRotation(FMath::Lerp(myOwnComponent->GetActorQuat(), targetRotation, 0.01f));
	}
	else
	{
		myOwnComponent->SetActorRelativeRotation(FMath::Lerp(myOwnComponent->GetActorQuat(), originRotation, 0.01f));
	}
}
