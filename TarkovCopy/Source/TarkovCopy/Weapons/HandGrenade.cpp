// Fill out your copyright notice in the Description page of Project Settings.


#include "HandGrenade.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AHandGrenade::AHandGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHandGrenade::BeginPlay()
{
	Super::BeginPlay();
	sphereCollision = Cast<USphereComponent>(GetDefaultSubobjectByName(TEXT("ExplosionCollision")));
	DeactivateGrenade();
}
void AHandGrenade::NotifyActorBeginOverlap(AActor* Other)
{
	if (Other->ActorHasTag(TEXT("Enemy")) || Other->ActorHasTag(TEXT("Player")))
	{
		grenadeTargets.Add(Other);
	}
}

void AHandGrenade::NotifyActorEndOverlap(AActor* Other)
{
	if (Other->ActorHasTag(TEXT("Enemy")) || Other->ActorHasTag(TEXT("Player")))
	{
		grenadeTargets.Remove(Other);
	}
}

void AHandGrenade::ReactivateGrenade()
{
	SetActorHiddenInGame(false);

	sphereCollision->SetActive(true);
	isExploded = false;
}

void AHandGrenade::DeactivateGrenade()
{
	SetActorHiddenInGame(true);
	sphereCollision->SetActive(false);
	isExploded = true;
	grenadeTargets.Reset();
}


void AHandGrenade::ThrowGrenade(FVector pDir,FVector pStartPos)
{
	if(getMesh == nullptr)
		getMesh =Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Mesh")));
	getMesh->SetSimulatePhysics(false);
	getMesh->SetSimulatePhysics(true);
	getMesh->SetPhysicsLinearVelocity(pDir * impulseForceVal);
	SetActorLocation(pStartPos);
	isGrenadeTossed = true;
	
}

void AHandGrenade::Explode()
{
	FVector explodeStart = GetActorLocation();
	FVector dir;
	FHitResult hit;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	int currentCount = 0;
	for (int i = 0; i < grenadeTargets.Num(); i++)
	{
		dir = (grenadeTargets[i]->GetActorLocation() - explodeStart);
		if (GetWorld()->LineTraceSingleByChannel(hit, explodeStart, dir * explosionRadius, ECollisionChannel::ECC_Pawn))
		{
			//플레이어 인가 아니면 
			if (hit.Actor->ActorHasTag(TEXT("Player")))
			{
				APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(hit.GetActor());
				playerCharacter->TookDamage(explosionDamage, hit, GetActorLocation());
			}
			//에너미인가
			else if (hit.Actor->ActorHasTag(TEXT("Enemy")))
			{
				AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
				aiCharacter->TookDamage(explosionDamage, hit, grenadeOwner);
			}
			currentCount++;
		}

		if (explosionFragmentCounts <= currentCount)
			break;
	}
	SetActorHiddenInGame(true);
	sphereCollision->SetActive(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	isExploded = true;
	isGrenadeTossed = false;
}

bool AHandGrenade::IsActive()
{
	return !isExploded;
}

// Called every frame
void AHandGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isExploded)
	{
		explosionTimer += DeltaTime;
		if (explosionTimer > explosionDuration)
		{
			explosionTimer = 0.f;
			Explode();
		}
	}
}

