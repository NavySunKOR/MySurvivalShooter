// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashGrenade.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

void AFlashGrenade::Explode()
{
	UE_LOG(LogTemp,Warning,TEXT("Explode Flash Grenade DRAWING : %s"), *GetActorLocation().ToString())
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation() + FVector(0, 0, 100.f), explosionRadius, 12, FLinearColor::Red, 50.f, 50.f);

	//ApplyRadialDamage�� �⺻������ DamageCauser���Ͱ� �ɸ��� �������� �������̴�.
	UGameplayStatics::ApplyRadialDamage(GetWorld(), flashDuration , GetActorLocation() + FVector(0, 0, 100.f), explosionRadius, UFlashDamageType::StaticClass(), TArray<AActor*>(), this, GetInstigator()->GetController(),true,ECollisionChannel::ECC_Visibility);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	DeactivateGrenade();
}