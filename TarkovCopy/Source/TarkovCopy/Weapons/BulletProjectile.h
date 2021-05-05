// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

class UStaticMeshComponent;
UCLASS()
class TARKOVCOPY_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
private:
	float damage;
	float velocity;
	float mass;
	bool isFired = false;
	bool isHitted = false;
	FVector lastShooterPos;
	FVector shootDir;
	UPROPERTY()
	APawn* bulletOwner;
	UPROPERTY()
	UStaticMeshComponent* getMesh;
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitTerrainParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* hitEnemyParticle;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitTerrainSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitEnemySound;

	UPROPERTY(EditAnywhere, Category="Collision")
	float rayCheckFrame;

	float rayCheckLength = 150.f;
	float rayCheckInterval = 0.f;
	float rayCheckTimer = 0.f;
	
	FVector rayCheckStartPos;
	FVector rayCheckEndPos;
	FCollisionQueryParams rayCheckParam;
	FHitResult rayCheckHitResult;

	void ObjectHit(AActor* Other, FHitResult Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
public:	
	// Called every frame
	ABulletProjectile();
	void LaunchProjectile();
	void ReactivateProjectile(float pDamage, float pVelocity,float pMass, APawn* pShooter, FVector pShootDir);
	void DeactivateProjectile();
	bool IsFired();
	virtual void Tick(float DeltaTime) override;

};
