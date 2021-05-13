// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HandGrenade.generated.h"

class USphereComponent;

UCLASS()
class TARKOVCOPY_API AHandGrenade : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandGrenade();
private:
	float explosionTimer;

	UPROPERTY()
	UStaticMeshComponent* getMesh;
protected:
	TArray<AActor*> grenadeTargets;
	USphereComponent* sphereCollision;

	UPROPERTY()
	bool isExploded = true;
	bool isGrenadeTossed = false;

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
	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

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
