// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Blueprint/UserWidget.h>
#include <DrawDebugHelpers.h>
#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemFlashGrenade.h"
#include "TarkovCopy/Interactable/InteractableObject.h"
#include "TarkovCopy/Interactable/InteractableComponent.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "TarkovCopy/Player/Components/PlayerStatusComponent.h"
#include "TarkovCopy/Player/Components/PlayerMovementComponent.h"
#include "TarkovCopy/Player/Components/PlayerEquipmentComponent.h"
#include "TarkovCopy/Player/Components/PlayerInventoryComponent.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/Utils/JsonSaveAndLoader.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}


void APlayerCharacter::SetInputSprint()
{
	PlayerMovementComponent->SetSprinting();
}

void APlayerCharacter::SetInputWalk()
{
	PlayerMovementComponent->SetWalking();
}

void APlayerCharacter::SetInputCrouch()
{
	PlayerMovementComponent->SetCrouch();
}

void APlayerCharacter::SetInputUncrouch()
{
	PlayerMovementComponent->SetUncrouch();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	helmetMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Helmet")));
	gameMode = Cast<ATarkovCopyGameModeBase>(GetWorld()->GetAuthGameMode());
	springArm = FindComponentByClass<USpringArmComponent>();
	springArmOrigin = GetMesh()->GetRelativeLocation();
	playerController = Cast<AFPPlayerController>(GetController());
	PlayerStatusComponent = Cast<UPlayerStatusComponent>(GetDefaultSubobjectByName(TEXT("PlayerStatus")));
	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetDefaultSubobjectByName(TEXT("PlayerMovement")));
	PlayerEquipmentComponent = Cast<UPlayerEquipmentComponent>(GetDefaultSubobjectByName(TEXT("PlayerEquipment")));
	PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(GetDefaultSubobjectByName(TEXT("PlayerInventory")));
	PlayerStatusComponent->Init(playerController, this);
	PlayerMovementComponent->Init(playerController, this,PlayerStatusComponent->DefaultSprintingSpeed
		, PlayerStatusComponent->DefaultWalkingSpeed
		, PlayerStatusComponent->DefaultAdsWalkingSpeed);
	PlayerEquipmentComponent->Init(playerController, this);
	PlayerInventoryComponent->Init(playerController, this);
	

	
	//Pool initialize;
}

UInventory* APlayerCharacter::GetInventory()
{
	return PlayerInventoryComponent->GetInventory();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	PlayerMovementComponent->Loop();
	
	if (isFirePressed)
	{
		if(!IsCloseToWall())
			PlayerEquipmentComponent->FireWeapon();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetInputSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SetInputWalk);
	PlayerInputComponent->BindAction(TEXT("EquipPrimary"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipPrimary);
	PlayerInputComponent->BindAction(TEXT("EquipSecondary"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipSecondary);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &APlayerCharacter::FireUpWeapon);
	PlayerInputComponent->BindAction(TEXT("SetWeaponSelector"), EInputEvent::IE_Released, this, &APlayerCharacter::SetWeaponSelector);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetADSWeapon);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Released, this, &APlayerCharacter::SetHipfireWeapon);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetInputCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &APlayerCharacter::SetInputUncrouch);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Inventory);
	PlayerInputComponent->BindAction(TEXT("InspectWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InspectWeapon);
	PlayerInputComponent->BindAction(TEXT("ThrowGrenade"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ThrowGrenade);
	PlayerInputComponent->BindAction(TEXT("ThrowFlashGrenade"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ThrowFlashGrenade);

	PlayerInputComponent->BindAxis(TEXT("Tilting"), this, &APlayerCharacter::Tilting);
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &APlayerCharacter::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &APlayerCharacter::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &APlayerCharacter::RotateVertical);
}


float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	APawn* instigatorPawn = EventInstigator->GetPawn();

	if (instigatorPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TakeDamage's instigatorPawn is null!"))
			return -1;
	}


	UE_LOG(LogTemp, Warning, TEXT("TakeDamage"))

		//TODO:이 비교문이 맞는지 체크 해 볼것.
		if (DamageEvent.DamageTypeClass == UFlashDamageType::StaticClass())
		{
			playerController->GetFlashed(Damage, DamageCauser->GetActorLocation());
		}
		else
		{
			FPointDamageEvent* damageEvent = (FPointDamageEvent*)&DamageEvent;

			//총상인지 아닌지 판단. 포인트 데미지면 총상이다.
			if (damageEvent->IsOfType(FPointDamageEvent::ClassID))
			{
				int generated = rand() % 10;
				if (generated < 3) // 0,1,2 중 하나 걸리니까 30퍼센트 헤드샷
				{
					Damage *= 2.5f;
					PlayerInventoryComponent->ReduceHelmetDurability(Damage);
				}
			}

			PlayerStatusComponent->DecreaseHealth(Damage);
			playerController->ShowHitIndicator(instigatorPawn->GetActorLocation());
		}



	return Damage;
}


bool APlayerCharacter::PickupItem(UItemInfo* pItemInfo)
{
	return PlayerInventoryComponent->PickupItem(pItemInfo);
}

void APlayerCharacter::StartMoveItemPos(UItemInfo* pItemInfo)
{
	PlayerInventoryComponent->StartMoveItemPos(pItemInfo);
}

bool APlayerCharacter::CanItemMoveTo(FSlateRect pIntSlateRect)
{
	return PlayerInventoryComponent->CanItemMoveTo(pIntSlateRect);
}

void APlayerCharacter::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	PlayerInventoryComponent->MoveItemTo(pItemInfo, pIntSlateRect);
}

void APlayerCharacter::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	PlayerInventoryComponent->FailedToMoveItemPos(pItemInfo);
}

void APlayerCharacter::Tilting(float pValue)
{
	FRotator quat;
	quat.Roll = GetActorRotation().Roll + pValue * 20.f;
	GetMesh()->SetWorldRotation(quat);
	springArm->SetRelativeRotation(GetMesh()->GetRelativeRotation());

	FVector originVec = springArmOrigin;
	originVec.Y = springArmOrigin.Y + pValue * 75.f;
	GetMesh()->SetRelativeLocation(originVec);

}

void APlayerCharacter::AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	if (PlayerEquipmentComponent->AddPrimary(pWeaponOrigin, pItemWeapon))
		PlayerInventoryComponent->RemoveItem((UItemInfo*)pItemWeapon);
}

void APlayerCharacter::AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	if (PlayerEquipmentComponent->AddSecondary(pWeaponOrigin, pItemWeapon))
		PlayerInventoryComponent->RemoveItem((UItemInfo*)pItemWeapon);
}

void APlayerCharacter::RemovePrimary()
{
	PlayerEquipmentComponent->RemovePrimary();
}

void APlayerCharacter::RemoveSecondary()
{
	PlayerEquipmentComponent->RemoveSecondary();
}

void APlayerCharacter::FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir)
{
	PlayerEquipmentComponent->FireProjectile(pDamage, pVelocity, pMass, pFireStartPos, pShootDir);
}

ABaseGun* APlayerCharacter::GetCurrentActiveGun()
{
	if (PlayerEquipmentComponent)
		return PlayerEquipmentComponent->GetCurrentEquipedWepaon();
	else
		return nullptr;
}

void APlayerCharacter::AddHelmet(UItemHelmet* pHelmetInfo)
{
	//TODO: 메쉬 찾아서 켜줘
	helmetMesh->SetVisibility(true);
	PlayerInventoryComponent->AddHelmet(pHelmetInfo);
}

void APlayerCharacter::RemoveHelmet(UItemHelmet* pHelmetInfo)
{
	helmetMesh->SetVisibility(false);
	PlayerInventoryComponent->RemoveHelmet();
}

void APlayerCharacter::SaveEquipments()
{
	PlayerInventoryComponent->SaveEquipments();
}

bool APlayerCharacter::IsWeaponEquiped()
{
	return PlayerEquipmentComponent&&PlayerEquipmentComponent->IsWeaponEquiped();
}

bool APlayerCharacter::IsAds()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->IsAds();
}

bool APlayerCharacter::IsWalking()
{
	return (PlayerMovementComponent && !PlayerMovementComponent->IsSprinting && GetVelocity().Size()>0) ;
}

int APlayerCharacter::GetWeaponCode()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->GetWeaponCode();
}

bool APlayerCharacter::IsEmptyMagazine()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->IsEmptyMagazine();
}

bool APlayerCharacter::IsCloseToWall()
{
	return isCloseToWall;
}

bool APlayerCharacter::IsShotgun()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->IsShotgun();
}

int APlayerCharacter::GetReloadState()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->GetReloadState();
}

bool APlayerCharacter::IsSprinting()
{
	return (PlayerMovementComponent && PlayerMovementComponent->IsSprinting && GetVelocity().Size() > 0);
}

bool APlayerCharacter::IsFiring()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->IsFiring();
}

bool APlayerCharacter::IsReloading()
{
	return PlayerEquipmentComponent && PlayerEquipmentComponent->IsReloading();
}

void APlayerCharacter::MoveVertical(float pValue)
{
	PlayerMovementComponent->MoveVerticalValue = pValue;
}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	PlayerMovementComponent->MoveHorizontalValue = pValue;
}

void APlayerCharacter::RotateHorizontal(float pValue)
{
	AddControllerYawInput(pValue);
}

void APlayerCharacter::RotateVertical(float pValue)
{
	AddControllerPitchInput(-pValue);
}

void APlayerCharacter::EquipPrimary()
{
	PlayerEquipmentComponent->EquipPrimary();
}

void APlayerCharacter::EquipSecondary()
{
	PlayerEquipmentComponent->EquipSecondary();
}

void APlayerCharacter::FireWeapon()
{
	if (playerController->isInventoryOpened || gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened)
	{
		return;
	}

	isFirePressed = true;
}



void APlayerCharacter::ActualFireWeapon()
{
	PlayerEquipmentComponent->ActualFireWeapon();
}

void APlayerCharacter::FireUpWeapon()
{
	isFirePressed = false;
	PlayerEquipmentComponent->UnlockFireWeapon();
}

void APlayerCharacter::SetADSWeapon()
{
	if (playerController->isInventoryOpened)
	{
		return;
	}

	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;

	if (IsCloseToWall())
		return;

	PlayerEquipmentComponent->SetADSWeapon();
	playerController->SetADS();
}

void APlayerCharacter::SetHipfireWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;

	PlayerEquipmentComponent->SetHipfireWeapon();
	playerController->SetHipfire();
}

void APlayerCharacter::ReloadWeapon()
{

	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;
	if (PlayerInventoryComponent->IsUsingInventory())
		return;

	if (PlayerEquipmentComponent->CanReloadWeapon())
	{
		int needAmmo = PlayerEquipmentComponent->GetNeedAmmoForReload();
		if (needAmmo > 0)
		{
			FString weaponName = PlayerEquipmentComponent->GetCurrentEquipedWepaon()->GetClass()->GetName();
			if (PlayerInventoryComponent->HaveEnoughAmmoForReload(weaponName, PlayerEquipmentComponent->IsPrimaryWeaponEquiped(), needAmmo))
				return;

			if (PlayerEquipmentComponent->IsPrimaryWeaponEquiped())
			{
				PlayerInventoryComponent->UsePrimaryAmmo(needAmmo, weaponName);
			}
			else
			{
				PlayerInventoryComponent->UseSecondaryAmmo(needAmmo, weaponName);
			}

			PlayerEquipmentComponent->ActualReloadWeapon(needAmmo);
		}
	}

	playerController->UpdateInventoryUI();

}

void APlayerCharacter::SetWeaponSelector()
{
	PlayerEquipmentComponent->SetWeaponSelector();
}

void APlayerCharacter::Interact()
{
	if (playerController->isInventoryOpened || gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened)
	{
		return;
	}
	FHitResult hit;
	FVector start;
	FRotator dir;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	APlayerController* con = Cast<APlayerController>(GetController());
	if (con == nullptr)
		return;
	con->GetPlayerViewPoint(start, dir);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, start + dir.Vector() * 800.f, ECollisionChannel::ECC_Pawn, param))
	{
		AActor* hitActor = hit.GetActor();
		AInteractableObject* inter = Cast<AInteractableObject>(hitActor);
		if (inter != nullptr)
		{
			inter->Interact();
		}
		else
		{
			UInteractableComponent* interComp = hitActor->FindComponentByClass<UInteractableComponent>();
			if (interComp != nullptr)
			{
				interComp->Interact(GetActorLocation());
			}
		}
	}
}

void APlayerCharacter::InspectWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;

	if (IsSprinting())
		return;

	PlayerEquipmentComponent->InspectWeapon();
}

void APlayerCharacter::Inventory()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;
	if (IsAds())
	{
		SetHipfireWeapon();
	}

	playerController->OpenCloseInventory();
}

void APlayerCharacter::ThrowGrenade()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;

	UItemInfo* itemReference = nullptr;
	if (!PlayerInventoryComponent->HasItemType(itemReference,ItemType::GRENADE))
	{
		return;
	}

	PlayerEquipmentComponent->ThrowGrenade();
	PlayerInventoryComponent->UseItem(itemReference);
}

void APlayerCharacter::ThrowFlashGrenade()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;
	UItemInfo* itemReference = nullptr;

	if (!PlayerInventoryComponent->HasItemType(itemReference, ItemType::FLASHGRENADE))
	{
		return;
	}

	PlayerEquipmentComponent->ThrowFlashGrenade();
	PlayerInventoryComponent->UseItem(itemReference);
}


void APlayerCharacter::HealPlayer(float pHealAmount)
{
	PlayerStatusComponent->HealPlayer(pHealAmount);
}