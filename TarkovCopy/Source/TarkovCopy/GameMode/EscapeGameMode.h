// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopyGameModeBase.h"
#include "EscapeGameMode.generated.h"

/**
 * 
 */
class AQuestItem;
class AFPPlayerController;
class ATriggerBox;
UCLASS()
class TARKOVCOPY_API AEscapeGameMode : public ATarkovCopyGameModeBase
{
	GENERATED_BODY()

private:
	//Quest
	TArray<AQuestItem*> allQuestItems;
	AQuestItem* activeQuestItem;
	bool isQuestCompleted = false;

	//Exfil
	TArray<AActor*> allExfilPoints;
	AActor* activeExfilPoint;
	AFPPlayerController* playerCon;

	bool isInitializedComplete = false;

protected:
	virtual void Tick(float DeltaSeconds) override;
public:
	void SelectQuestItems();
	void SelectExfilPoint();
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* pPlayerCon) override;
	virtual void QuestCompleted(AInteractableObject* questItem) override;
	virtual void TryExfil() override;
	virtual void CancelExfil() override;
	
};
