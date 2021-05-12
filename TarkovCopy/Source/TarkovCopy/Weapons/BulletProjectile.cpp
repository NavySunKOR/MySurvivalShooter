// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletProjectile.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABulletProjectile::LaunchProjectile()
{
	getMesh->SetSimulatePhysics(false);
	getMesh->SetSimulatePhysics(true);
	getMesh->SetPhysicsLinearVelocity(shootDir * velocity);
}

void ABulletProjectile::ReactivateProjectile(float pDamage, float pVelocity, float pMass,APawn* pShooter,FVector pShootDir)
{
	isHitted = false;
	isFired = true;
	damage = pDamage;
	velocity = pVelocity;
	mass = pMass;
	lastShooterPos = pShooter->GetActorLocation();
	bulletOwner = pShooter;
	shootDir = pShootDir;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ABulletProjectile::DeactivateProjectile()
{
	isFired = false;
	getMesh->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

bool ABulletProjectile::IsFired()
{
	return isFired;
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	getMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("BulletMesh")));
	rayCheckInterval = 1.f / rayCheckFrame;
	DeactivateProjectile();
}

void ABulletProjectile::ObjectHit(AActor* Other, FHitResult Hit)
{
	if (bulletOwner->GetName().Equals(Other->GetName()))
		return;
	AAICharacter* aiCharacter = Cast<AAICharacter>(Hit.GetActor());
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(Hit.GetActor());
	if (aiCharacter != nullptr)
	{
		//TODO: �Ѿ��� ���ʴ� �÷��̾� �Ǵ� AI�� ĳ���ͷ� ���� �� �ٰ�.
		aiCharacter->TookDamage(damage, Hit, GetOwner());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEnemyParticle, Hit.ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitEnemySound, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	//�ƴϸ� ����ó��.
	else if (playerCharacter != nullptr)
	{
		playerCharacter->TookDamage(damage, Hit, lastShooterPos);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEnemyParticle, Hit.ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitEnemySound, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitTerrainParticle, Hit.ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitTerrainSound, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}

	DeactivateProjectile();
}

void ABulletProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!isHitted)
	{
		isHitted = true;
		ObjectHit(Other, Hit);
	}
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CCD�ε� ������Ÿ���� ������ �հ� ���� ������ �߻�. Raycast�� �߰� üũ
	if (isFired)
	{
		rayCheckTimer += DeltaTime;
		if (rayCheckTimer > rayCheckInterval)
		{
			rayCheckParam.ClearIgnoredActors();
			rayCheckParam.AddIgnoredActor(this);
			rayCheckParam.AddIgnoredActor(bulletOwner);
			rayCheckTimer = 0.f;
			rayCheckStartPos = GetActorLocation();
			rayCheckEndPos = rayCheckStartPos + shootDir * rayCheckLength;
			DrawDebugLine(GetWorld(), rayCheckStartPos, rayCheckEndPos, FColor::Orange);
			if (GetWorld()->LineTraceSingleByChannel(rayCheckHitResult, rayCheckStartPos, rayCheckEndPos, ECollisionChannel::ECC_Pawn, rayCheckParam))
			{
				if (!isHitted)
				{
					isHitted = true;
					ObjectHit(rayCheckHitResult.GetActor(), rayCheckHitResult);
				}
			}
		}
	}
}

