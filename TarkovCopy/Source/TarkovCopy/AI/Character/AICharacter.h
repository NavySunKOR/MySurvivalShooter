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

public:	
	// Called every frame
	FVector outPlayerLocation;
	bool outIsPlayerDetected = false;
	bool isDead = false;
	bool isFlashed = false;
	float flashInterval = 0.f;
	float flashTimer = 0.f;
	
	UPROPERTY()
	AActor* trackingTarget; // ���� �Ҷ� ���(�߼Ҹ� ��� �ش� �������� �̵�)
	UPROPERTY()
	AActor* targetActor; //���� �������� ���� �߰� �� ���� �� �߻� �Ҷ� ���

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> bulletProjectileOrigin;

	virtual void Tick(float DeltaTime) override;
	void GetFlashed(float pFlashDuration);
	void TookDamage(float pDamageAmount, FHitResult pHitParts, AActor* pShooter);
	void SetActiveFalse();
	float GetCurrentWeaponRange();
	void FireWeapon();
	void FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
