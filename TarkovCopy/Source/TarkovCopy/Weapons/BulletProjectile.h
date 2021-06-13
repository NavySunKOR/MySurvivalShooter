// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"	
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class TARKOVCOPY_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
private:
	float damage;
	float velocity;
	float mass;
	bool isFired = false;
	FVector lastShooterPos;
	FVector shootDir;
	APawn* bulletOwner;



	UPROPERTY(VisibleAnywhere,Category="Component")
	USphereComponent* sphereComponent;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UProjectileMovementComponent* projectileMovementComponent;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitTerrainParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitEnemyParticle;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitTerrainSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitEnemySound;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	ABulletProjectile();
	void LaunchProjectile();
	void ReactivateProjectile(float pDamage, float pVelocity,float pMass, APawn* pShooter, FVector pShootDir);
	void DeactivateProjectile();
	bool IsFired();

};
