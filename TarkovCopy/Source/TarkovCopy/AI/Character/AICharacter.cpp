// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/SphereComponent.h>
#include "TarkovCopy/AI/Controller/UserCreatedAIController.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "AICharacter.h"

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
	ATarkovCopyGameModeBase* gameMode = GetWorld()->GetAuthGameMode<ATarkovCopyGameModeBase>();
	int selectedWeapon = FMath::RandRange(0, gameMode->allAIGunsInGame.Num() - 1);
	currentActiveGun = GetWorld()->SpawnActor<ABaseGun>(gameMode->allAIGunsInGame[selectedWeapon]);
	aiController = Cast<AUserCreatedAIController>(GetController());

	if (currentActiveGun != nullptr)
	{
		currentActiveGun->SetParentMeshTPP(GetMesh());
		currentActiveGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Hand_R_Position"));
		currentActiveGun->SetActorRelativeLocation(currentActiveGun->thirdPersonPosition);
		currentActiveGun->SetActorRotation(FRotator(0,0,0));
		currentActiveGun->weaponOwnerAICharacter = this;
		//currentActiveGun->SetActorRelativeRotation(GetMesh()->GetSocketRotation(TEXT("Hand_R_Position")) + currentActiveGun->thirdPersonRotation);
		currentActiveGun->SetOwner(this);

	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("HELP! HELP!"))
	}
	curHp = maxHp;
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::TookDamage(float pDamageAmount, FHitResult pHitParts)
{
	//hitParts에 따라서 pDamageAmount를 계산할것.
	curHp -= pDamageAmount;
	if (curHp <= 0)
	{
		//TODO:PawnKilled 넣을것.
		curHp = 0;
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::NotifyActorBeginOverlap(AActor* Other)
{
	if (Other->ActorHasTag(FName("Player")))
	{
		FVector targetDir = (Other->GetActorLocation() - GetOwner()->GetActorLocation());
		targetDir.Normalize();
		float angleCos = FVector::DotProduct(GetOwner()->GetActorForwardVector(), targetDir) / GetOwner()->GetActorForwardVector().Size() * targetDir.Size();
		float toAngle = FMath::RadiansToDegrees(FMath::Acos(angleCos));


		if (toAngle < 90.f)
		{
			outPlayerLocation = Other->GetActorLocation();
			outIsPlayerDetected = true;
			aiController->SetFocus(Other);
		}
	}
}
void AAICharacter::NotifyActorEndOverlap(AActor* Other)
{
	if (Other->ActorHasTag(FName("Player")))
	{
		outIsPlayerDetected = false;
		aiController->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

void AAICharacter::FireWeapon()
{
	FVector start = GetActorLocation();
	FVector dir = (targetActor->GetActorLocation() - GetActorLocation());
	currentActiveGun->FireWeapon(GetActorLocation(), dir.Rotation());
}
