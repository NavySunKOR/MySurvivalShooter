// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"


UCLASS()
class TARKOVCOPY_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
private:
	float damage;
	float velocity;
	float mass;
	FVector lastShooterPos;
	UPROPERTY()
	UStaticMeshComponent* mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	ABulletProjectile();
	void ReactivateProjectile(float pDamage, float pVelocity,float pMass, FVector pLastShooterPos, FVector pShootDir);
	void DeactivateProjectile();
	virtual void Tick(float DeltaTime) override;

};
