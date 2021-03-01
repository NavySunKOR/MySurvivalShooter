// Fill out your copyright notice in the Description page of Project Settings.


#include <Kismet/GameplayStatics.h>
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
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

	//TODO: 힙파이어냐 아니면 ads냐에 따라서 발사 방식을 다르게 하고, ads에서 발사시 총기는 선형 방식으로 반동을 적용(무기가 위로 올라가야함);
	if (GetWorld()->LineTraceSingleByChannel(hit, start, start + dir.Vector() * range, ECollisionChannel::ECC_Pawn, param))
	{
		//맞은 hit가 캐릭터면
		AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(hit.GetActor());
		if (aiCharacter != nullptr)
		{
			aiCharacter->TookDamage(damage, hit);
		}
		else if (playerCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit player"));
			playerCharacter->TookDamage(damage, hit);
		}
		//아니면 지형처리.
		else
		{
			//TODO: 나중에 지형에 코드를 삽입하고 해당 장소에 파편을 튀기는 방식으로 제작할것.
			UE_LOG(LogTemp, Warning, TEXT("Terrain~!"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrainParticle, hit.ImpactPoint, dir, true);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitTerrainSound, hit.ImpactPoint, dir, true);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Done!"));
	}

	if (isAds)
	{
		if(weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(aimFireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(aimFireAnim);
	}
	else
	{

		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(fireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(fireAnim);
	}
	Super::FireWeapon(start,dir);
}

void AM9::Reload(int pInsertMagazine)
{
	Super::Reload(pInsertMagazine);
	if (curMagRounds == 0)
	{
		reloadInterval = emptyReloadAnim->GetPlayLength() * 0.9f;

		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(emptyReloadAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(emptyReloadAnim);
		UGameplayStatics::SpawnSoundAttached(emptyReloadSound,weaponComponents);
	}
	else
	{
		reloadInterval = tacticalReloadAnim->GetPlayLength() * 0.9f;

		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(tacticalReloadAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(tacticalReloadAnim);
		UGameplayStatics::SpawnSoundAttached(tacticalReloadSound, weaponComponents);
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
