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
	for (int i = 0; i < grenadeTargets.Num(); i++)
	{
		dir = (grenadeTargets[i]->GetActorLocation() - explodeStart);
		if (GetWorld()->LineTraceSingleByChannel(hit, explodeStart, dir * explosionRadius, ECollisionChannel::ECC_Pawn))
		{
			//�÷��̾� �ΰ� �ƴϸ� 
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
			//���ʹ��ΰ�
			else if (hit.Actor->ActorHasTag(TEXT("Enemy")))
			{
				//AI���� ������ ����
				AAICharacter* aiCharacter = Cast<AAICharacter>(hit.GetActor());
				aiCharacter->GetFlashed(flashDuration);
				//aiCharacter->TookDamage(explosionDamage, hit, grenadeOwner);
			}
		}
	}
	SetActorHiddenInGame(true);
	sphereCollision->SetActive(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
	isExploded = true;
	isGrenadeTossed = false;
}