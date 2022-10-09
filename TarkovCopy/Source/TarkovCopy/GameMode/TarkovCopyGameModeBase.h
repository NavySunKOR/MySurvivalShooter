// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TarkovCopyGameModeBase.generated.h"

#define TABLE_CONTEXT TEXT("GENERAL")

/**
 * 
 */
class AAICharacter;
class UButton;
class UItemInfo;
class ABaseGun;
class AInteractableObject;
class AFPPlayerController;
class UDataTable;

UENUM()
enum class ETableType : uint8
{
	NONE = 0,
	WEAPON
}
;

UENUM()
enum class AntiAlliasingOptionType : uint8
{
	OFF UMETA(DisplayName = "Off"),
	FXAA UMETA(DisplayName = "FXAA"),
	MSAA UMETA(DisplayName = "MSAA"),
	TAA UMETA(DisplayName="TAA")
};
UENUM()
enum class OnOffType : uint8
{
	OFF UMETA(DisplayName = "Off"),
	ON UMETA(DisplayName = "On")
};

UCLASS()
class TARKOVCOPY_API ATarkovCopyGameModeBase : public AGameMode
{
	GENERATED_BODY()
public:
	ATarkovCopyGameModeBase();
private:
	TArray<AAICharacter*> aiPlayers;
	TSubclassOf<AAICharacter> aiCharacter;

	//pause menu buttons

	void InitializeAI();
	void ReturnToMainMenu();


protected:
	UPROPERTY(EditAnywhere, Category = "Weapons")
	UDataTable* WeaeponTable;
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	USoundBase* diedSound;
	UPROPERTY(EditAnywhere)
	USoundBase* escapedSound;

	template<typename T>
	T* GetTableData(ETableType pTableType, int32 pKey);

	UPROPERTY()
	AFPPlayerController* playerController;


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

