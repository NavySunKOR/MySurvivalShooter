// Fill out your copyright notice in the Description page of Project Settings.


#include <Kismet/GameplayStatics.h>
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "M416.h"

void AM416::BeginPlay()
{
	itemCode = 4;
	maximumMagRounds = 30;
	reloadInterval = 0.f;
	rpm = 750.f;
	damage = 25.f;
	range = 15000.f;
	thirdPersonScale = FVector(1.f, 1.f, 1.f);
	thirdPersonRotation = FRotator(0.f, 0.f, 0.f);
	thirdPersonPosition = FVector(2.f, 10.f, 20.f);
	Super::BeginPlay();

}

bool AM416::CanFireWeapon()
{
	return Super::CanFireWeapon();
}


void AM416::FireWeapon(FVector start,FRotator dir)
{
	FHitResult hit;
	FCollisionQueryParams param;

	GetWorld()->DebugDrawTraceTag = FName("Cong");
	param.AddIgnoredActor(this);
	APawn* player = Cast<APawn>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("Owner thing : %s"), *player->GetName());
	param.AddIgnoredActor(player);
	param.TraceTag = FName("Cong");

	//TODO: 힙파이어냐 아니면 ads냐에 따라서 발사 방식을 다르게 하고, ads에서 발사시 총기는 선형 방식으로 반동을 적용(무기가 위로 올라가야함);
	if (GetWorld()->LineTraceSingleByChannel(hit, start, start + dir.Vector() * range, ECollisionChannel::ECC_Pawn , param))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit something : %s"), *hit.GetActor()->GetName());
		//맞은 hit가 캐릭터면
		AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(hit.GetActor());
		if (aiCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("NAME : %s"), *aiCharacter->GetName());
			aiCharacter->TookDamage(damage, hit);
		}
		//아니면 지형처리.
		else if (playerCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit player mamag"));
			//playerCharacter->TookDamage(damage, hit);
		}
		else
		{
			//TODO: 나중에 지형에 코드를 삽입하고 해당 장소에 파편을 튀기는 방식으로 제작할것.

			UE_LOG(LogTemp, Warning, TEXT("Terrain~!"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrainParticle, hit.ImpactPoint, dir);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitTerrainSound, hit.ImpactPoint, dir, true);

		}

		UE_LOG(LogTemp, Warning, TEXT("Done!"));
	}


	if (isAds)
	{
		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(aimFireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(aimFireAnim);
	}
	else
	{
		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(fireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(aimFireAnim);
	}
	Super::FireWeapon(start, dir);

}

void AM416::Reload(int pInsertMagazine)
{
	Super::Reload(pInsertMagazine);
	if (curMagRounds == 0)
	{
		reloadInterval = emptyReloadAnim->GetPlayLength();
		if(weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(emptyReloadAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(emptyReloadAnim);
		UGameplayStatics::SpawnSoundAttached(emptyReloadSound, weaponComponents);
	}
	else
	{
		reloadInterval = emptyReloadAnim->GetPlayLength();
		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(tacticalReloadAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(tacticalReloadAnim);
		UGameplayStatics::SpawnSoundAttached(tacticalReloadSound, weaponComponents);
	}
}

void AM416::SetADS()
{
	isAds = true;
}

void AM416::SetHipfire()
{
	isAds = false;
}

void AM416::Tick(float DeltaTime)
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
