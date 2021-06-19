// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatusComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TARKOVCOPY_API UPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatusComponent();
private:

	UPROPERTY(EditAnywhere)
	float MaxHp = 100;
	float CurHp;
	
	AFPPlayerController* PlayerController;
	APlayerCharacter* PlayerCharacter;

	void Loop();

public:	

	UPROPERTY(EditAnywhere)
	float DefaultSprintingSpeed;
	UPROPERTY(EditAnywhere)
	float DefaultWalkingSpeed;
	UPROPERTY(EditAnywhere)
	float DefaultAdsWalkingSpeed;


	void Init();
	void End();

	void HealPlayer(float pHealAmount);
	void DecreaseHealth(float pDamageAmount);




//Inlines
public :
	FORCEINLINE void SetPlayerInformation(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
	{
		PlayerController = pPlayerController;
		PlayerCharacter = pPlayerCharacter;
	}
	FORCEINLINE bool IsDead() { return CurHp <= 0; };
		
};
