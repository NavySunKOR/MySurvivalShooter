// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/Interactable/InteractableComponent.h"
#include "OpendoorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TARKOVCOPY_API UOpendoorComponent : public UInteractableComponent
{
	GENERATED_BODY()
private: 
	bool isDoorOpened = false;
	float openDoorAngle = 90.f;
	FQuat originRotation;
	FQuat targetRotation;
	class AActor* myOwnComponent;

protected:
	virtual void BeginPlay() override;
public:
	virtual void Interact() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
