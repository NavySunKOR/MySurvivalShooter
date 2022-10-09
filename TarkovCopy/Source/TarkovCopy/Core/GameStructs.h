// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameStructs.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponStatus
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		int16 WeaponId;
	UPROPERTY(EditAnywhere)
		float Rpm;
	UPROPERTY(EditAnywhere)
		int MaximumMagRounds;
	UPROPERTY(EditAnywhere)
		float Damage;
	UPROPERTY(EditAnywhere)
		float Range;
	UPROPERTY(EditAnywhere)
		float BulletVelocity;
	UPROPERTY(EditAnywhere)
		float BulletMass;
	UPROPERTY(EditAnywhere)
		float InitBulletSpreadRadius;
	UPROPERTY(EditAnywhere)
		float MaxBulletSpreadRadius;
	UPROPERTY(EditAnywhere)
		float BulletSpreadIncrement;
	UPROPERTY(EditAnywhere)
		float BulletSpreadDecrementPerSecond;

	UPROPERTY(EditAnywhere)
		float AdsBulletHorizontalSpreadIncrement;
	UPROPERTY(EditAnywhere)
		float AdsBulletVerticalSpreadIncrement;

	UPROPERTY(EditAnywhere)
		bool IsCanAutoFire;
};

UCLASS()
class TARKOVCOPY_API UGameStruct : public UObject {
	GENERATED_BODY()

};


