// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TarkovCopy/Interactable/InteractableObject.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopyGameModeBase.generated.h"

/**
 * 
 */
class AAICharacter;
UCLASS()
class TARKOVCOPY_API ATarkovCopyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATarkovCopyGameModeBase();
private:
	void ReturnToMainMenu();
	TArray<AAICharacter*> aiPlayers;
	TSubclassOf<AAICharacter> aiCharacter;

	void InitalizeAI();
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	USoundBase* diedSound;
	UPROPERTY(EditAnywhere)
	USoundBase* escapedSound;

	


	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allPlayerGunsInGame;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allAIGunsInGame;
	
	bool isPlayerDied = false;
	bool isPlayerEscaped = false;

	virtual void PlayerDied();
	virtual void QuestCompleted(AInteractableObject* questItem);
	virtual void ExfilCompleted();
	virtual void TryExfil();
	virtual void CancelExfil();

};
