// Fill out your copyright notice in the Description page of Project Settings.


#include "M9.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemWeapon.h"
#include <Kismet/GameplayStatics.h>

AM9::AM9()
{
}

void AM9::BeginPlay()
{
	fppAnimBlueprints = LoadObject<UAnimBlueprintGeneratedClass>(NULL, TEXT("AnimBlueprint'/Game/Animations/Arms/ABP_Anim_M9.ABP_Anim_M9_C'"), NULL, LOAD_None, NULL);
	tppAnimBlueprints = LoadObject<UAnimBlueprintGeneratedClass>(NULL, TEXT("AnimBlueprint'/Game/Animations/TPS/ABP_Anim_TPC_Handgun.ABP_Anim_TPC_Handgun_C'"), NULL, LOAD_None, NULL);
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

	FVector endPoint = start + dir.Vector() * range;
	if (!isAds)
	{
		endPoint += FVector(FMath::RandRange(-calcBulletSpreadRadius, calcBulletSpreadRadius), 0.f, FMath::RandRange(-calcBulletSpreadRadius, calcBulletSpreadRadius));
	}
	FVector directionToFly = (endPoint - start).GetSafeNormal();

	if (weaponOwnerCharacter)
	{
		weaponOwnerCharacter->FireProjectile(damage,bulletVelocity,bulletMass, start, directionToFly);
	}
	else
	{
		weaponOwnerAICharacter->FireProjectile(damage, bulletVelocity, bulletMass, start, directionToFly);
	}


	//if (GetWorld()->LineTraceSingleByChannel(hit, start, endPoint, ECollisionChannel::ECC_Pawn, param))
	//{
	//	//맞은 hit가 캐릭터면
	//	AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
	//	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(hit.GetActor());
	//	if (aiCharacter != nullptr)
	//	{
	//		aiCharacter->TookDamage(damage, hit, GetOwner());
	//	}
	//	else if (playerCharacter != nullptr)
	//	{
	//		FVector shooterPos = (weaponOwnerCharacter) ? weaponOwnerCharacter->GetActorLocation() : weaponOwnerAICharacter->GetActorLocation();
	//		playerCharacter->TookDamage(damage, hit, shooterPos);
	//	}
	//	//아니면 지형처리.
	//	else
	//	{
	//		//TODO: 나중에 지형에 코드를 삽입하고 해당 장소에 파편을 튀기는 방식으로 제작할것.
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrainParticle, hit.ImpactPoint, dir, true);
	//		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitTerrainSound, hit.ImpactPoint, dir, true);
	//	}
	//}

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
		reloadInterval = emptyReloadAnim->GetPlayLength() * 0.6f;

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
			itemWeapon->currentMagazineAmmo = curMagRounds;
			isReloading = false;
			reloadTimer = 0.f;
		}
	}
}
