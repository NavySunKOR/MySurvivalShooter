// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandGrenade.h"
#include "FlashGrenade.generated.h"


class UDamageEvent;

UCLASS()
class UFlashDamageType : public UDamageType
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API AFlashGrenade : public AHandGrenade
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	float flashDuration;
public:
	virtual void Explode() override;
};
