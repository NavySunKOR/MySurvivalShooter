// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashGrenade.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

void AFlashGrenade::Explode()
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
				if (playerCharacter != nullptr)
				{
					AFPPlayerController* playerController = playerCharacter->playerController;
					FVector myLoc = GetActorLocation();
					playerController->GetFlashed(flashDuration, myLoc);
				}
			}
			//에너미인가
			else if (hit.Actor->ActorHasTag(TEXT("Enemy")))
			{
				//AI에게 적용할 사항
				AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
				//aiCharacter->TookDamage(explosionDamage, hit, grenadeOwner);
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