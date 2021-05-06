// Fill out your copyright notice in the Description page of Project Settings.


#include "OpendoorComponent.h"

void UOpendoorComponent::BeginPlay()
{
	Super::BeginPlay();
	myOwnComponent = GetOwner();
	originRotation = myOwnComponent->GetActorQuat();
	targetRotation = originRotation;
}

void UOpendoorComponent::Interact(FVector pInteractFrom)
{
	//문이 완전히 열릴 때 까지 기다릴것.
	if (myOwnComponent->GetActorQuat().Equals(originRotation) || myOwnComponent->GetActorQuat().Equals(targetRotation))
	{

		directionFrom = (pInteractFrom - myOwnComponent->GetActorLocation()).GetSafeNormal();
		if (originRotation.Equals(myOwnComponent->GetActorQuat()))
		{
			float doorDir = FVector::DotProduct(directionFrom, myOwnComponent->GetActorForwardVector()) / directionFrom.Size() * myOwnComponent->GetActorForwardVector().Size();
			float playerDir = FVector::DotProduct(FVector::CrossProduct(directionFrom, myOwnComponent->GetActorForwardVector()), FVector::UpVector);
			UE_LOG(LogTemp, Warning, TEXT("Print out door dir : %f , playerDir : %f"), doorDir, playerDir);
			//건물 밖에서 안쪽으로 밀리는 문인가? 아니면 안에서 바깥쪽으로 나가는 문인가를 체크한다.
			float doorOrigin = 0.f;
			//dir이 크면 여는문 dir이 작으면 닫는 문
			if (doorDir >= 0)
			{
				doorOrigin = 1.f;
			}
			else if (doorDir < 0)
			{
				doorOrigin = -1.f;
			}

			//미는 방향을 기준으로 만들어놓음. 당기고 싶으면 doorOrigin의 부호를 바꾸면 됨

			targetRotation = (playerDir > 0) ? FQuat(originRotation.Rotator() + FRotator(0, 90.f * doorOrigin, 0)) : FQuat(originRotation.Rotator() + FRotator(0, 90.f * -doorOrigin, 0));
		}


		isDoorOpened = !isDoorOpened;
	}

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
