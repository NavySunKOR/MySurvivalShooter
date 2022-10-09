// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EvasionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TARKOVCOPY_API UEvasionGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
};