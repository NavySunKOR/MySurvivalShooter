// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "BasePlayerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

class TARKOVCOPY_API UBasePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasePlayerComponent();

protected:
	AFPPlayerController* PlayerController;
	APlayerCharacter* PlayerCharacter;

public:	
	virtual void Init(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter);
	virtual void Loop();
	virtual void End();
		
};
