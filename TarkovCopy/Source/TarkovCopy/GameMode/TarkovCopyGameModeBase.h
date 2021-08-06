// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TarkovCopyGameModeBase.generated.h"

/**
 * 
 */
class AAICharacter;
class UButton;
class UItemInfo;
class ABaseGun;
class AInteractableObject;
class AFPPlayerController;


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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> pauseMenuWidgetOrigin;
	UPROPERTY()
	UUserWidget* pauseMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> optionMenuWidgetOrigin;
	UPROPERTY()
	UUserWidget* optionMenuWidget;


	//pause menu buttons

	void InitializeAI();
	void InitializeSystemUI();
	void ReturnToMainMenu();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OpenPauseMenu();

	UFUNCTION(BlueprintCallable)
	void ClosePauseMenu();

	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	void OpenOptionMenu();

	UFUNCTION(BlueprintCallable)
	void CloseOptionMenu();

public:
	UPROPERTY(EditAnywhere)
	USoundBase* diedSound;
	UPROPERTY(EditAnywhere)
	USoundBase* escapedSound;


	UPROPERTY()
	AFPPlayerController* playerController;


	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allPlayerGunsInGame;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseGun>> allAIGunsInGame;
	
	bool isPlayerDied = false;
	bool isPlayerEscaped = false;
	bool isPauseMenuOpened = false;

	virtual void PlayerDied();
	virtual void QuestCompleted(AInteractableObject* questItem);
	virtual void ExfilCompleted();
	virtual void TryExfil();
	virtual void CancelExfil();




};
