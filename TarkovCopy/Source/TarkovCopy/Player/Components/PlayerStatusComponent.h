// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePlayerComponent.h"
#include "PlayerStatusComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TARKOVCOPY_API UPlayerStatusComponent : public UBasePlayerComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatusComponent();
private:

	UPROPERTY(EditAnywhere)
	float MaxHp = 100;
	float CurHp;

public:	

	UPROPERTY(EditAnywhere)
	float DefaultSprintingSpeed;
	UPROPERTY(EditAnywhere)
	float DefaultWalkingSpeed;
	UPROPERTY(EditAnywhere)
	float DefaultAdsWalkingSpeed;

	void InitStatusComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter);
	void HealPlayer(float pHealAmount);
	void DecreaseHealth(float pDamageAmount);

//Inlines
public :

	FORCEINLINE bool IsDead() { return CurHp <= 0; };
		
};
