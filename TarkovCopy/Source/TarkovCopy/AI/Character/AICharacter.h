// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>
#include "AICharacter.generated.h"

class AUserCreatedAIController;
class ABulletProjectile;
class ABaseGun;
class USphereComponent;
class ACharacter;

UCLASS()
class TARKOVCOPY_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called when take damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	float maxHp = 100.f;
	float curHp;


	USphereComponent* detectTrigger;
	ABaseGun* currentActiveGun;
	AUserCreatedAIController* aiController;
	TArray<ABulletProjectile*> bulletProjectilePools;


	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;
	void Dead();
	void CalculateDamageAmount(float& pDamage, FHitResult& pHitParts);

public:	
	// Called every frame
	FVector outPlayerLocation;
	bool outIsPlayerDetected = false;
	bool isDead = false;
	bool isFlashed = false;
	float flashInterval = 0.f;
	float flashTimer = 0.f;
	
	UPROPERTY()
	AActor* trackingTarget; // 추적 할때 사용(발소리 듣고 해당 현장으로 이동)
	UPROPERTY()
	AActor* targetActor; //실제 육안으로 적을 발견 후 조준 후 발사 할때 사용

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> bulletProjectileOrigin;

	virtual void Tick(float DeltaTime) override;
	void GetFlashed(float pFlashDuration);
	void SetActiveFalse();
	float GetCurrentWeaponRange();
	void FireWeapon();
	void FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
