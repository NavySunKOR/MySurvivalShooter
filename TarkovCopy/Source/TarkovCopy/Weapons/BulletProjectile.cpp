// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletProjectile.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABulletProjectile::ReactivateProjectile(float pDamage, float pVelocity, float pMass, FVector pLastShooterPos,FVector pShootDir)
{
	if (mesh == nullptr)
		mesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Mesh")));
	damage = pDamage;
	velocity = pVelocity;
	mass = pMass;
	lastShooterPos = pLastShooterPos;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	mesh->SetSimulatePhysics(true);
	mesh->SetPhysicsLinearVelocity(pShootDir * pVelocity);
}

void ABulletProjectile::DeactivateProjectile()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	mesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	DeactivateProjectile();
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

