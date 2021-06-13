// Fill out your copyright notice in the Description page of Project Settings.


#include "HandGrenade.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AHandGrenade::AHandGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

}

// Called when the game starts or when spawned
void AHandGrenade::BeginPlay()
{
	Super::BeginPlay();
	DeactivateGrenade();
}

void AHandGrenade::ReactivateGrenade()
{
	SetActorHiddenInGame(false); 
	isExploded = false;
}

void AHandGrenade::DeactivateGrenade()
{
	SetActorHiddenInGame(true);
	isExploded = true;
}


void AHandGrenade::ThrowGrenade(FVector pDir,FVector pStartPos) 
{
	ReactivateGrenade();
	SetActorLocation(pStartPos);
	projectileMovement->InitialSpeed = impulseForceVal / 2.f;
	projectileMovement->MaxSpeed = impulseForceVal;
	projectileMovement->ProjectileGravityScale = 3.f;
	projectileMovement->Velocity = pDir * impulseForceVal;

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AHandGrenade::Explode, explosionDuration);
}

void AHandGrenade::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Explode Hand Grenade DRAWING Position : %s"), *GetActorLocation().ToString())
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0, 0, 100.f), explosionRadius,12,FLinearColor::Red,50.f,50.f);

	//ApplyRadialDamage는 기본적으로 DamageCauser액터가 걸리면 데미지가 미적용이다.
	UGameplayStatics::ApplyRadialDamage(GetWorld(), explosionDamage, GetActorLocation() + FVector(0,0,100.f), explosionRadius,UDamageType::StaticClass(), TArray<AActor*>(),this,GetInstigator()->GetController(), false, ECollisionChannel::ECC_Visibility);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	DeactivateGrenade();
}

bool AHandGrenade::IsActive()
{
	return !isExploded;
}

// Called every frame
void AHandGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

