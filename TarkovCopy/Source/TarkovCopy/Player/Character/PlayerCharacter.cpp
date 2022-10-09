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
#include "TarkovCopy/GameMode/EscapeGameMode.h"
#include "TarkovCopy/Player/Components/PlayerStatusComponent.h"
#include "TarkovCopy/Player/Components/PlayerMovementComponent.h"
#include "TarkovCopy/Player/Components/PlayerEquipmentComponent.h"
#include "TarkovCopy/Player/Components/PlayerInventoryComponent.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "TarkovCopy/Weapons/FlashGrenade.h"
#include "TarkovCopy/UI/InGameHUD.h"
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
	gameMode = Cast<AEscapeGameMode>(GetWorld()->GetAuthGameMode());
	springArm = FindComponentByClass<USpringArmComponent>();
	springArmOrigin = GetMesh()->GetRelativeLocation();
	playerController = Cast<AFPPlayerController>(GetController());
	PlayerStatusComponent = Cast<UPlayerStatusComponent>(GetDefaultSubobjectByName(TEXT("PlayerStatus")));
	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetDefaultSubobjectByName(TEXT("PlayerMovement")));
	PlayerEquipmentComponent = Cast<UPlayerEquipmentComponent>(GetDefaultSubobjectByName(TEXT("PlayerEquipment")));
	PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(GetDefaultSubobjectByName(TEXT("PlayerInventory")));
	PlayerStatusComponent->InitStatusComponent(playerController, this);
	PlayerEquipmentComponent->InitEquipmentComponent(playerController, this);
	PlayerInventoryComponent->InitInventoryComponent(playerController, this);
	PlayerMovementComponent->InitMovementComponent(playerController, this, PlayerStatusComponent->DefaultSprintingSpeed
		, PlayerStatusComponent->DefaultWalkingSpeed
		, PlayerStatusComponent->DefaultAdsWalkingSpeed);

	IngameHUD = Cast<AInGameHUD>(playerController->GetHUD());
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
	if (isFirePressed && !isCloseToWall)
	{
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
	PlayerInputComponent->BindAction(TEXT("PauseMenu"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PauseMenu);
	
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

	//TODO:이 비교문이 맞는지 체크 해 볼것.
	bool isDamageTypeFlashbang = DamageEvent.DamageTypeClass == UFlashDamageType::StaticClass();

	if (isDamageTypeFlashbang)
	{
		playerController->GetInGameHUD()->GetFlashed(Damage, DamageCauser->GetActorLocation());
	}
	else
	{
		FPointDamageEvent* damageEvent = (FPointDamageEvent*)&DamageEvent;
		bool isGunshotWounded = damageEvent->IsOfType(FPointDamageEvent::ClassID); //총상인지 아닌지 판단. 포인트 데미지면 총상이다.

		if (isGunshotWounded)
		{
			int generated = rand() % 10;
			bool isUnder30percent = generated < 3; // 0,1,2 중 하나 걸리니까 30퍼센트 헤드샷
			if (isUnder30percent) 
			{
				Damage *= 2.5f;
				PlayerInventoryComponent->ReduceHelmetDurability(Damage);
			}
		}

		PlayerStatusComponent->DecreaseHealth(Damage);
		playerController->GetInGameHUD()->ShowHitIndicator(instigatorPawn->GetActorLocation());
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
	bool isAddPrimarySuccessful = PlayerEquipmentComponent->AddPrimary(pWeaponOrigin, pItemWeapon);
	if (isAddPrimarySuccessful)
		PlayerInventoryComponent->RemoveItem((UItemInfo*)pItemWeapon);
}

void APlayerCharacter::AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	bool isAddSecondarySuccessful = PlayerEquipmentComponent->AddSecondary(pWeaponOrigin, pItemWeapon);
	if (isAddSecondarySuccessful)
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
	if (PlayerMovementComponent == nullptr)
		return false;
	
	return PlayerEquipmentComponent->IsWeaponEquiped();
}

bool APlayerCharacter::IsAds()
{
	if (PlayerMovementComponent == nullptr)
		return false;

	return PlayerEquipmentComponent->IsAds();
}

bool APlayerCharacter::IsWalking()
{
	if (PlayerMovementComponent == nullptr)
		return false;

	bool isSprinting = PlayerMovementComponent->IsSprinting;
	bool hasSpeed = GetVelocity().Size() > 0;

	return (!isSprinting && hasSpeed);
}

int APlayerCharacter::GetWeaponCode()
{
	if (PlayerMovementComponent == nullptr)
		return -1;
	return PlayerEquipmentComponent->GetWeaponCode();
}

bool APlayerCharacter::IsEmptyMagazine()
{
	if (PlayerMovementComponent == nullptr)
		return false;

	return PlayerEquipmentComponent->IsEmptyMagazine();
}

bool APlayerCharacter::IsCloseToWall()
{
	return isCloseToWall;
}

bool APlayerCharacter::IsShotgun()
{
	if (PlayerMovementComponent == nullptr)
		return false;
	return PlayerEquipmentComponent->IsShotgun();
}

int APlayerCharacter::GetReloadState()
{
	if (PlayerMovementComponent == nullptr)
		return -1;
	return PlayerEquipmentComponent->GetReloadState();
}

bool APlayerCharacter::IsSprinting()
{
	if (PlayerMovementComponent == nullptr)
		return false;

	return PlayerMovementComponent->IsSprinting && GetVelocity().Size() > 0;
}

bool APlayerCharacter::IsFiring()
{
	if (PlayerEquipmentComponent == nullptr)
		return false;

	return PlayerEquipmentComponent->IsFiring();
}

bool APlayerCharacter::IsReloading()
{
	if (PlayerEquipmentComponent == nullptr)
		return false;

	return PlayerEquipmentComponent->IsReloading();
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
	//인벤토리가 열렸는지?
	if (playerController->isInventoryOpened)
		return;

	//액션이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
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
	//인벤토리가 열렸는지?
	if (playerController->isInventoryOpened)
	{
		return;
	}

	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
		return;

	//벽과 가까운지?
	if (isCloseToWall)
		return;

	PlayerEquipmentComponent->SetADSWeapon();
	playerController->GetInGameHUD()->SetCrosshairInvisible();
}

void APlayerCharacter::SetHipfireWeapon()
{
	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
		return;

	PlayerEquipmentComponent->SetHipfireWeapon();
	playerController->GetInGameHUD()->SetCrosshairVisible();
}

void APlayerCharacter::ReloadWeapon()
{
	// 행동이 가능한지?
	bool isActionable = IsActionable(); 
	if (!isActionable)
		return;

	// 인벤토리 사용 중인지?
	bool isUsingInventory = PlayerInventoryComponent->IsUsingInventory(); 
	if (isUsingInventory)
		return;

	// 무기를 장전 할 수 있는지?
	bool canReloadWeapon = PlayerEquipmentComponent->CanReloadWeapon(); 
	if (!canReloadWeapon)
		return;

	// 무기를 장전해야하는지?
	int needAmmo = PlayerEquipmentComponent->GetNeedAmmoForReload();
	bool isNeedAmmoForWeapon = needAmmo > 0; 
	if (!isNeedAmmoForWeapon)
		return;

	//무기를 장전하기 위하여 충분한 탄약이 있는지?
	FString weaponName = PlayerEquipmentComponent->GetCurrentEquipedWepaon()->GetClass()->GetName();
	bool haveEnoughAmmoForReload = PlayerInventoryComponent->HaveEnoughAmmoForReload(weaponName,
		PlayerEquipmentComponent->IsPrimaryWeaponEquiped(), needAmmo); 
	if (!haveEnoughAmmoForReload)
		return;


	//주무기를 장착 중인지?
	bool isPrimaryWeaponEquiped = PlayerEquipmentComponent->IsPrimaryWeaponEquiped();
	if (isPrimaryWeaponEquiped)
	{
		PlayerInventoryComponent->UsePrimaryAmmo(needAmmo, weaponName);
	}
	else
	{
		PlayerInventoryComponent->UseSecondaryAmmo(needAmmo, weaponName);
	}

	PlayerEquipmentComponent->ActualReloadWeapon(needAmmo);
	playerController->UpdateInventoryUI();

}

void APlayerCharacter::SetWeaponSelector()
{
	PlayerEquipmentComponent->SetWeaponSelector();
}

void APlayerCharacter::Interact()
{
	//인벤토리가 열렸는지
	if (playerController->isInventoryOpened)
	{
		return;
	}

	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
	{
		return;
	}

	//컨트롤러 null일 시 패스
	APlayerController* con = Cast<APlayerController>(GetController());
	if (con == nullptr)
		return;

	FHitResult hit;
	FVector start;
	FRotator dir;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	con->GetPlayerViewPoint(start, dir);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, start + dir.Vector() * 800.f, ECollisionChannel::ECC_WorldDynamic, param))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit name %s"), *hit.Actor->GetName())
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
	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
		return;

	//달리고 있는지?
	bool isSprinting = IsSprinting();
	if (isSprinting)
		return;

	PlayerEquipmentComponent->InspectWeapon();
}

void APlayerCharacter::Inventory()
{
	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
		return;

	//조준 중인가?
	bool isAds = IsAds();
	if (isAds)
	{
		SetHipfireWeapon();
	}

	playerController->OpenCloseInventory();
}

void APlayerCharacter::PauseMenu()
{
	IngameHUD->OpenClosePauseMenu();
}

void APlayerCharacter::ThrowGrenade()
{
	//행동이 가능한지?
	bool isActionable = IsActionable();
	if (!isActionable)
		return;

	//아이템을 가지고 있는지?
	UItemInfo* itemReference = nullptr;
	bool hasItemType = PlayerInventoryComponent->HasItemType(itemReference, ItemType::GRENADE);
	if (!hasItemType)
	{
		return;
	}

	PlayerEquipmentComponent->ThrowGrenade();
	PlayerInventoryComponent->UseItem(itemReference);
}

void APlayerCharacter::ThrowFlashGrenade()
{
	//행동이 가능한가?
	bool isActionable = IsActionable();
	if (!isActionable)
	{
		return;
	};
	
	//아이템이 사용이 가능한가?
	UItemInfo* itemReference = nullptr;
	bool hasItem = PlayerInventoryComponent->HasItemType(itemReference, ItemType::FLASHGRENADE);
	if (!hasItem)
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

bool APlayerCharacter::IsActionable() const
{
	//gamemode가 null인지?
	if (gameMode == nullptr)
		return false;
	
	//플레이어가 사망 했는지?
	bool isPlayerDied = gameMode->isPlayerDied;
	if (isPlayerDied)
		return false;

	//플레이어가 탈출 했는지?
	bool isPlayerEscaped = gameMode->isPlayerEscaped;
	if (isPlayerEscaped)
		return false;

	//메뉴가 열렸는지?
	bool isPauseMenuOpened = IngameHUD && IngameHUD->IsPauseMenuOpened();
	if (isPauseMenuOpened)
		return false;
	else
		return true;
}