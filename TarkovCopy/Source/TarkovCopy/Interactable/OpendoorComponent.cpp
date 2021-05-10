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
			//문 자체가 건물 밖에서 안쪽으로 밀리는 문인가? 아니면 안에서 바깥쪽으로 나가는 문인가를 체크한다.
			float doorDir = FVector::DotProduct(directionFrom, myOwnComponent->GetActorForwardVector()) / directionFrom.Size() * myOwnComponent->GetActorForwardVector().Size();
			//플레이어 기준으로 문을 밀어서 열때 어느 방향에서 여는지 체크(예: 건물 밖 - 안 또는 건물 안 - 밖)
			float playerDir = FVector::DotProduct(FVector::CrossProduct(directionFrom, myOwnComponent->GetActorForwardVector()), FVector::UpVector);
			UE_LOG(LogTemp, Warning, TEXT("Print out door dir : %f , playerDir : %f"), doorDir, playerDir);
			float doorOrigin = 0.f;
			//dir이 크면 여는문 dir이 작으면 닫는 문
			doorOrigin = (doorDir >= 0)? 1.f:-1.f;
			//미는 방향으로 방향 기준을 만들어놓음. 당기고 싶으면 doorOrigin의 부호를 바꾸면 됨
			targetRotation = (playerDir > 0) ? FQuat(originRotation.Rotator() + FRotator(0, 90.f * doorOrigin, 0)) : FQuat(originRotation.Rotator() + FRotator(0, 90.f * -doorOrigin, 0));
		}

		/*
			설명 예 : 
			내가 건물 밖에서 안으로 들어갈려고 할때 문을 밀어서 안으로 진입 하고 싶다.
			문자체가 건물 안에서 바깥으로 열리는 문인데 내가 들어가는 방향은 밖에서 안이다. 근데 만약에 문을 그냥 열면 안에서 바깥으로 열릴것이다. 그러므로 targetRotation적용 전에 doorOrigin에 음수를 붙여 뒤집으면 바깥에서 안으로 열릴 것이다.
		*/

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
