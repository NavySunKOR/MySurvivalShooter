// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>
#include "TarkovCopy/AI/Controller/UserCreatedAIController.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "TarkovCopy/Weapons/BulletProjectile.h"

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



// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (outIsPlayerDetected)
	{
		outPlayerLocation = trackingTarget->GetActorLocation();
	}

	//Ž�� ���� ������ ���Դµ� �ǹ� ���̰ų� �ƴϸ� ���� ������������ �ľ�
	if (trackingTarget != nullptr && !outIsPlayerDetected)
	{
		if (aiController != nullptr && aiController->LineOfSightTo(trackingTarget))
		{
			outPlayerLocation = trackingTarget->GetActorLocation();
			aiController->SetFocus(trackingTarget);
			outIsPlayerDetected = true;
		}
	}
}

void CalculateDamageAmount(float& pDamage,FHitResult& pHitParts)
{
	//Head shot
	if (pHitParts.BoneName.ToString().Equals(TEXT("head")) || pHitParts.BoneName.ToString().Equals(TEXT("neck")))
	{
		pDamage *= 2.5f;
		//TODO:���߿� �� �ý��� ������ ������ �氨 ������ ��.
	}
	else if(pHitParts.BoneName.ToString().Equals(TEXT("hips")) 
		|| pHitParts.BoneName.ToString().Equals(TEXT("spine"))
		|| pHitParts.BoneName.ToString().Equals(TEXT("chest"))) //Body shot
	{
		pDamage *= 1.5f;
		//TODO:���߿� �� �ý��� ������ ������ �氨 ������ ��.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("shoulder"))
		|| pHitParts.BoneName.ToString().Contains(TEXT("arm"))
		) //Shoulder and arm
	{
		pDamage *=0.85f;
		//TODO:���߿� �� �ý��� ������ ������ �氨 ������ ��.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("leg"))) //Leg and thigh
	{
		pDamage *= 1.25f;
		//TODO:���߿� �� �ý��� ������ ������ �氨 ������ ��.
	}
	else if (pHitParts.BoneName.ToString().Contains(TEXT("hand"))
		|| pHitParts.BoneName.ToString().Contains(TEXT("foot")))
	{
		pDamage *= 0.5f;
		//TODO:���߿� �� �ý��� ������ ������ �氨 ������ ��.
	}

}

void AAICharacter::TookDamage(float pDamageAmount, FHitResult pHitParts,AActor* pShooter)
{
	//hitParts�� ���� pDamageAmount�� ����Ұ�.
	CalculateDamageAmount(pDamageAmount, pHitParts);

	curHp -= pDamageAmount;

	NotifyActorBeginOverlap(pShooter);
	if (curHp <= 0)
	{
		//TODO:PawnKilled ������.
		curHp = 0;
		Dead();
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
		/*FVector targetDir = (Other->GetActorLocation() - GetActorLocation());
		targetDir.Normalize();
		float angleCos = FVector::DotProduct(GetOwner()->GetActorForwardVector(), targetDir) / GetOwner()->GetActorForwardVector().Size() * targetDir.Size();
		float toAngle = FMath::RadiansToDegrees(FMath::Acos(angleCos));*/

		trackingTarget = Other;
		if (aiController->LineOfSightTo(trackingTarget))
		{
			outPlayerLocation = trackingTarget->GetActorLocation();
			aiController->SetFocus(Other);
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
			aiController->ClearFocus(EAIFocusPriority::Default);
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
		bulletProjectilePools.Add(GetWorld()->SpawnActor<ABulletProjectile>(bulletProjectileOrigin));
		bulletProjectilePools[bulletProjectilePools.Num() -1]->ReactivateProjectile(pDamage, pVelocity, pMass,Cast<APawn>(this), pShootDir);
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->LaunchProjectile();
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->SetActorLocation(pFireStartPos);
		isProjectileFired = true;
	}
}