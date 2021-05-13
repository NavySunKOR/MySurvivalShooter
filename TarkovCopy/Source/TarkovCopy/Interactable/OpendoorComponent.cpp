// Fill out your copyright notice in the Description page of Project Settings.


#include "OpendoorComponent.h"

void UOpendoorComponent::BeginPlay()
{
	Super::BeginPlay();
	myOwnComponent = GetOwner();
	originRotation = myOwnComponent->GetActorQuat();
	targetRotation = FRotator(0.f, 90.f, 0.f).Quaternion();
}

void UOpendoorComponent::Interact(FVector pInteractFrom)
{
	//문이 완전히 열릴 때 까지 기다릴것.
	isDoorOpened = !isDoorOpened;
}

void UOpendoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isDoorOpened)
	{
		
		myOwnComponent->SetActorRelativeRotation(FMath::Lerp(myOwnComponent->GetActorQuat(), targetRotation, 0.1f));
	}
	else
	{
		myOwnComponent->SetActorRelativeRotation(FMath::Lerp(myOwnComponent->GetActorQuat(), originRotation, 0.1f));
	}
}
