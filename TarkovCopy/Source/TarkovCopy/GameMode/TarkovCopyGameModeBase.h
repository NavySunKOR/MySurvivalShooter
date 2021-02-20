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
UCLASS()
class TARKOVCOPY_API ATarkovCopyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allPlayerGunsInGame;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allAIGunsInGame;
	/*UPROPERTY(EditAnywhere)
	TArray<FString> allThirdPersonAnimInGame;*/
	UPROPERTY(EditAnywhere)
	TArray<UAnimBlueprint*> allThirdPersonAnimInGame;



	UPROPERTY(EditAnywhere)
	TSubclassOf <UItemInfo> itemM416;
	UPROPERTY(EditAnywhere)
	TSubclassOf <UItemInfo> itemM9;

	virtual void QuestCompleted(AInteractableObject* questItem);
	virtual void TryExfil();
	virtual void CancelExfil();

};
