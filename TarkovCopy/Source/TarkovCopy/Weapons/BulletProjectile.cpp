// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletProjectile.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	FScriptDelegate bulletHIt;
	bulletHIt.BindUFunction(this, TEXT("ObjectHit"));

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
	SetRootComponent(sphereComponent);

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	projectileMovementComponent->bSweepCollision = true;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	staticMeshComponent->AttachToComponent(sphereComponent, FAttachmentTransformRules::KeepWorldTransform);

}

void ABulletProjectile::LaunchProjectile()
{
	projectileMovementComponent->SetUpdatedComponent(sphereComponent);
	projectileMovementComponent->InitialSpeed = velocity/2;
	projectileMovementComponent->MaxSpeed = velocity;
	projectileMovementComponent->bRotationFollowsVelocity = true;
	projectileMovementComponent->Velocity = shootDir * projectileMovementComponent->InitialSpeed;
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
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABulletProjectile::DeactivateProjectile()
{
	isFired = false;
	SetActorHiddenInGame(true);
	sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABulletProjectile::IsFired()
{
	return isFired;
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	sphereComponent->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnHit);
	DeactivateProjectile();
}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bulletOwner->GetName().Equals(OtherActor->GetName()))
		return;
	if(OtherActor->IsA(APlayerCharacter::StaticClass()) || OtherActor->IsA(AAICharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hiited Enemy"));
		UGameplayStatics::ApplyPointDamage(OtherActor, damage, NormalImpulse, Hit, GetInstigatorController(), bulletOwner, UDamageType::StaticClass());
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

