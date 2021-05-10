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
	//���� ������ ���� �� ���� ��ٸ���.
	if (myOwnComponent->GetActorQuat().Equals(originRotation) || myOwnComponent->GetActorQuat().Equals(targetRotation))
	{

		directionFrom = (pInteractFrom - myOwnComponent->GetActorLocation()).GetSafeNormal();
		if (originRotation.Equals(myOwnComponent->GetActorQuat()))
		{
			//�� ��ü�� �ǹ� �ۿ��� �������� �и��� ���ΰ�? �ƴϸ� �ȿ��� �ٱ������� ������ ���ΰ��� üũ�Ѵ�.
			float doorDir = FVector::DotProduct(directionFrom, myOwnComponent->GetActorForwardVector()) / directionFrom.Size() * myOwnComponent->GetActorForwardVector().Size();
			//�÷��̾� �������� ���� �о ���� ��� ���⿡�� ������ üũ(��: �ǹ� �� - �� �Ǵ� �ǹ� �� - ��)
			float playerDir = FVector::DotProduct(FVector::CrossProduct(directionFrom, myOwnComponent->GetActorForwardVector()), FVector::UpVector);
			UE_LOG(LogTemp, Warning, TEXT("Print out door dir : %f , playerDir : %f"), doorDir, playerDir);
			float doorOrigin = 0.f;
			//dir�� ũ�� ���¹� dir�� ������ �ݴ� ��
			doorOrigin = (doorDir >= 0)? 1.f:-1.f;
			//�̴� �������� ���� ������ ��������. ���� ������ doorOrigin�� ��ȣ�� �ٲٸ� ��
			targetRotation = (playerDir > 0) ? FQuat(originRotation.Rotator() + FRotator(0, 90.f * doorOrigin, 0)) : FQuat(originRotation.Rotator() + FRotator(0, 90.f * -doorOrigin, 0));
		}

		/*
			���� �� : 
			���� �ǹ� �ۿ��� ������ ������ �Ҷ� ���� �о ������ ���� �ϰ� �ʹ�.
			����ü�� �ǹ� �ȿ��� �ٱ����� ������ ���ε� ���� ���� ������ �ۿ��� ���̴�. �ٵ� ���࿡ ���� �׳� ���� �ȿ��� �ٱ����� �������̴�. �׷��Ƿ� targetRotation���� ���� doorOrigin�� ������ �ٿ� �������� �ٱ����� ������ ���� ���̴�.
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
