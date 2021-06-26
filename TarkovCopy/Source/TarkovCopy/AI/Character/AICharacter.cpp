// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "TarkovCopy/AI/Controller/UserCreatedAIController.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "TarkovCopy/Weapons/FlashGrenade.h"
#include "TarkovCopy/Weapons/BulletProjectile.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include <Components/SphereComponent.h>
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	detectTrigger = Cast< USphereComponent>(GetDefaultSubobjectByName(TEXT("Sphere")));
	ATarkovCopyGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ATarkovCopyGameModeBase>();
	curHp = maxHp;
	int selectedWeapon = FMath::RandRange(0, gameMode->allAIGunsInGame.Num() - 1);
	currentActiveGun = GetWorld()->SpawnActor<ABaseGun>(gameMode->allAIGunsInGame[selectedWeapon]);

	aiController = Cast<AUserCreatedAIController>(GetController());

	if (currentActiveGun != nullptr)
	{
		currentActiveGun->SetParentMeshTPP(GetMesh());
		currentActiveGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Hand_R_Position"));
		currentActiveGun->SetActorRelativeLocation(currentActiveGun->thirdPersonPosition);
		currentActiveGun->SetActorRotation(FRotator(0, 0, 0));
		currentActiveGun->weaponOwnerAICharacter = this;
		currentActiveGun->SetOwner(this);
	}
}

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//hitParts에 따라서 pDamageAmount를 계산할것.
	APawn* instigatorPawn = EventInstigator->GetPawn();
	if (instigatorPawn == nullptr)
		return -1;


	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));

	//TODO:이 비교문이 맞는지 체크 해 볼것.
	if (DamageEvent.DamageTypeClass == UFlashDamageType::StaticClass())
	{
		GetFlashed(Damage);
	}
	else
	{
		FPointDamageEvent* damageEvent = (FPointDamageEvent*)&DamageEvent;

		UE_LOG(LogTemp, Warning, TEXT("FPointDamageEvent"));
		//총에 맞았으면 부위별 데미지를, 수류탄에 맞았으면 그냥 통짜로 데미지가 들어감.
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			CalculateDamageAmount(Damage, damageEvent->HitInfo);
		}

		UE_LOG(LogTemp, Warning, TEXT("CalculateDamageAmount"));

		curHp -= Damage;
		NotifyActorBeginOverlap(instigatorPawn);
		if (curHp <= 0)
		{
			curHp = 0;
			Dead();
		}
	}

	return Damage;
}



// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead)
		return;

	if (outIsPlayerDetected)
	{
		outPlayerLocation = trackingTarget->GetActorLocation();
	}

	//탐지 범위 내에는 들어왔는데 건물 안이거나 아니면 벽에 가려져있을때 파악
	if (!outIsPlayerDetected)
	{
		if (trackingTarget && aiController && aiController->LineOfSightTo(trackingTarget))
		{
			outPlayerLocation = trackingTarget->GetActorLocation();
			aiController->SetFocus(trackingTarget);
			UE_LOG(LogTemp,Warning,TEXT("Tracking bogey"))
			outIsPlayerDetected = true;
		}
	}

	if (isFlashed)
	{
		if(aiController)
			aiController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		flashTimer += DeltaTime;
		if (flashTimer > flashInterval)
		{
			isFlashed = false;
			if(trackingTarget)
			aiController->SetFocus(trackingTarget);
		}
	}
}

void AAICharacter::GetFlashed(float pFlashDuration)
{
	flashInterval = pFlashDuration;
	flashTimer = 0.f;
	isFlashed = true;
}

void AAICharacter::CalculateDamageAmount(float& pDamage,FHitResult& pHitParts)
{
	//Head shot
	if (pHitParts.BoneName.ToString().Equals(TEXT("head")) || pHitParts.BoneName.ToString().Equals(TEXT("neck")))
	{
		pDamage *= 2.5f;
		//TODO:나중에 방어구 시스템 나오면 데미지 경감 시켜줄 것.
	}
	else if(pHitParts.BoneName.ToString().Equals(TEXT("hips")) 
		|| pHitParts.BoneName.ToString().Equals(TEXT("spine"))
		|| pHitParts.BoneName.ToString().Equals(TEXT("chest"))) //Body shot
	{
		pDamage *= 1.5f;
		//TODO:나중에 방어구 시스템 나오면 데미지 경감 시켜줄 것.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("shoulder"))
		|| pHitParts.BoneName.ToString().Contains(TEXT("arm"))
		) //Shoulder and arm
	{
		pDamage *=0.85f;
		//TODO:나중에 방어구 시스템 나오면 데미지 경감 시켜줄 것.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("leg"))) //Leg and thigh
	{
		pDamage *= 1.25f;
		//TODO:나중에 방어구 시스템 나오면 데미지 경감 시켜줄 것.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("hand"))
		|| pHitParts.BoneName.ToString().Contains(TEXT("foot")))
	{
		pDamage *= 0.5f;
		//TODO:나중에 방어구 시스템 나오면 데미지 경감 시켜줄 것.
	}

}

float AAICharacter::GetCurrentWeaponRange()
{
	return currentActiveGun->range;
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::NotifyActorBeginOverlap(AActor* Other)
{
	
	if (!isDead && Other != nullptr &&  Other->ActorHasTag(TEXT("Player")))
	{
		trackingTarget = Other;
		if (aiController->LineOfSightTo(trackingTarget))
		{
			outPlayerLocation = trackingTarget->GetActorLocation();
			if (!isFlashed)
			{
				aiController->SetFocus(Other);
				UE_LOG(LogTemp, Warning, TEXT("Tracking bogey in detection"))
			}
			outIsPlayerDetected = true;
		}
	}
}
void AAICharacter::NotifyActorEndOverlap(AActor* Other)
{
	if (!isDead && Other != nullptr && Other->ActorHasTag(TEXT("Player")))
	{
		outIsPlayerDetected = false;
		if(aiController)
			aiController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		trackingTarget = nullptr;
	}
}

void AAICharacter::Dead()
{
	detectTrigger->SetActive(false);
	GetCharacterMovement()->StopMovementImmediately();
	FindComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(TEXT("NoColision"));
	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAnimInstanceClass(nullptr);
	GetMesh()->SetSimulatePhysics(true);
	DetachFromControllerPendingDestroy();
	isDead = true;
	aiController = nullptr;

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AAICharacter::SetActiveFalse, 2.f);
}

void AAICharacter::SetActiveFalse()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	currentActiveGun->SetActorEnableCollision(false);
	currentActiveGun->SetActorHiddenInGame(true);
	SetActorHiddenInGame(true);
}

void AAICharacter::FireWeapon()
{
	if (currentActiveGun->CanFireWeapon())
	{
		FVector start = GetActorLocation() + GetActorForwardVector() * 150.f;
		FVector dir = (targetActor->GetActorLocation() - start);
		currentActiveGun->FireWeapon(start, dir.Rotation());
	}
	else if (currentActiveGun->curMagRounds <= 0 && !currentActiveGun->isReloading){
		currentActiveGun->Reload(currentActiveGun->maximumMagRounds - currentActiveGun->curMagRounds);
	}
}

void AAICharacter::FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir)
{
	bool isProjectileFired = false;
	for (int i = 0; i < bulletProjectilePools.Num(); i++)
	{
		if (!bulletProjectilePools[i]->IsFired())
		{
			bulletProjectilePools[i]->ReactivateProjectile(pDamage, pVelocity, pMass,Cast<APawn>(this), pShootDir);
			bulletProjectilePools[i]->LaunchProjectile();
			bulletProjectilePools[i]->SetActorLocation(pFireStartPos);
			isProjectileFired = true;
			break;
		}
	}

	if (!isProjectileFired)
	{
		ABulletProjectile* bullet = GetWorld()->SpawnActor<ABulletProjectile>(bulletProjectileOrigin);
		bullet->SetInstigator(this);
		bullet->SetOwner(this);
		bulletProjectilePools.Add(bullet);
		bulletProjectilePools[bulletProjectilePools.Num() -1]->ReactivateProjectile(pDamage, pVelocity, pMass,Cast<APawn>(this), pShootDir);
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->LaunchProjectile();
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->SetActorLocation(pFireStartPos);
		isProjectileFired = true;
	}
}