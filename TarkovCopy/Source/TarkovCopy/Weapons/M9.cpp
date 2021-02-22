// Fill out your copyright notice in the Description page of Project Settings.


#include <Kismet/GameplayStatics.h>
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "M9.h"

void AM9::BeginPlay()
{
	itemCode = 0;
	maximumMagRounds = 6;
	reloadInterval = 3.f;
	rpm = 100.f;
	damage = 15.f;
	range = 15000.f;
	thirdPersonScale = FVector(1.f, 1.f, 1.f);
	thirdPersonRotation = FRotator(0.f, 0.f, 0.f);
	thirdPersonPosition = FVector(-2.391582f, 3.731291f, 12.247958f);
	Super::BeginPlay();
}

bool AM9::CanFireWeapon()
{
	return Super::CanFireWeapon();
}

void AM9::FireWeapon(FVector start, FRotator dir)
{
	FHitResult hit;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	APawn* player = Cast<APawn>(GetOwner());
	param.AddIgnoredActor(player);

	//TODO: �����̾�� �ƴϸ� ads�Ŀ� ���� �߻� ����� �ٸ��� �ϰ�, ads���� �߻�� �ѱ�� ���� ������� �ݵ��� ����(���Ⱑ ���� �ö󰡾���);
	if (GetWorld()->LineTraceSingleByChannel(hit, start, start + dir.Vector() * range, ECollisionChannel::ECC_Pawn, param))
	{
		//���� hit�� ĳ���͸�
		AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
		if (aiCharacter != nullptr)
		{
			aiCharacter->TookDamage(damage, hit);
		}
		//�ƴϸ� ����ó��.
		else
		{
			//TODO: ���߿� ������ �ڵ带 �����ϰ� �ش� ��ҿ� ������ Ƣ��� ������� �����Ұ�.
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrain, hit.ImpactPoint, dir, true);
		}
	}
	Super::FireWeapon(start,dir);
}

void AM9::Reload(int pInsertMagazine)
{
	Super::Reload(pInsertMagazine);
	if (curMagRounds == 0)
	{
		reloadInterval = emptyReload->GetPlayLength();
		weaponOwnerCharacter->PlayAnimationMontage(emptyReload);
	}
	else
	{
		reloadInterval = tacticalReload->GetPlayLength();
		weaponOwnerCharacter->PlayAnimationMontage(tacticalReload);
	}

}

void AM9::SetADS()
{
	isAds = true;
}

void AM9::SetHipfire()
{
	isAds = false;
}

void AM9::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isFiring)
	{
		fireTimer += DeltaTime;
		if (fireTimer > fireInterval)
		{
			isFiring = false;
			fireTimer = 0.f;

		}
	}

	if (isReloading)
	{
		reloadTimer += DeltaTime;
		if (reloadTimer > reloadInterval)
		{
			curMagRounds += tempInsertMag;
			isReloading = false;
			reloadTimer = 0.f;
		}
	}
}
