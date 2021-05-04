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
	DeactivateProjectile();
}

void ABulletProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bulletOwner->GetName().Equals(Other->GetName()))
		return;
	AAICharacter* aiCharacter = Cast<AAICharacter>(Hit.GetActor());
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(Hit.GetActor());
	if (aiCharacter != nullptr)
	{
		//TODO: 총알의 오너는 플레이어 또는 AI의 캐릭터로 설정 해 줄것.
		aiCharacter->TookDamage(damage, Hit, GetOwner());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEnemyParticle, Hit.ImpactPoint);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), hitEnemySound, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	//아니면 지형처리.
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

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

