// Fill out your copyright notice in the Description page of Project Settings.


#include "M416.h"
#include <Kismet/GameplayStatics.h>
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/AI/Character/AICharacter.h"

AM416::AM416()
{
}

void AM416::BeginPlay()
{
	fppAnimBlueprints = LoadObject<UAnimBlueprintGeneratedClass>(NULL, TEXT("AnimBlueprint'/Game/Animations/Arms/ABP_Anim_M416.ABP_Anim_M416_C'"), NULL, LOAD_None, NULL);
	tppAnimBlueprints = LoadObject<UAnimBlueprintGeneratedClass>(NULL, TEXT("AnimBlueprint'/Game/Animations/TPS/ABP_Anim_TPC_Rifle.ABP_Anim_TPC_Rifle_C'"), NULL, LOAD_None, NULL);


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
	param.AddIgnoredActor(this);
	APawn* player = Cast<APawn>(GetOwner());
	param.AddIgnoredActor(player);

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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEnemyParticle, hit.ImpactPoint);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitEnemySound, hit.ImpactPoint,hit.ImpactNormal.Rotation());
		}
		//아니면 지형처리.
		else if (playerCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit player mamag"));
			playerCharacter->TookDamage(damage, hit);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEnemyParticle, hit.ImpactPoint);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitEnemySound, hit.ImpactPoint, hit.ImpactNormal.Rotation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Terrain~!"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrainParticle, hit.ImpactPoint);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitTerrainSound, hit.ImpactPoint, hit.ImpactNormal.Rotation());

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
		reloadInterval = emptyReloadAnim->GetPlayLength() * 0.9f;
		if(weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(emptyReloadAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(emptyReloadAnim);
		UGameplayStatics::SpawnSoundAttached(emptyReloadSound, weaponComponents);
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
