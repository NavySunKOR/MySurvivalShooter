// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandGrenade.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TARKOVCOPY_API AHandGrenade : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AHandGrenade();
private:
	float explosionTimer;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UProjectileMovementComponent* projectileMovement;

	UPROPERTY()
	UStaticMeshComponent* getMesh;
protected:
	bool isExploded = true;

	UPROPERTY(EditAnywhere)
	float explosionDuration;
	UPROPERTY(EditAnywhere)
	float explosionRadius;
	UPROPERTY(EditAnywhere)
	float explosionDamage;
	UPROPERTY(EditAnywhere)
	float impulseForceVal;
	UPROPERTY(EditAnywhere)
	UParticleSystem* explosionParticle; 
	UPROPERTY(EditAnywhere)
	USoundBase* explosionSound;

	virtual void BeginPlay() override;

public:	
	UPROPERTY()
	AActor* grenadeOwner;
	void ReactivateGrenade();
	void DeactivateGrenade();
	virtual void Explode();

	void ThrowGrenade(FVector pDir, FVector pStartPos);

	bool IsActive();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
