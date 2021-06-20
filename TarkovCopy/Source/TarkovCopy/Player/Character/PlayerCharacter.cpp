// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Blueprint/UserWidget.h>
#include <DrawDebugHelpers.h>
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemHelmet.h"
#include "TarkovCopy/InventoryAndItem/GameFunctions/Inventory.h"
#include "TarkovCopy/Interactable/InteractableObject.h"
#include "TarkovCopy/Interactable/InteractableComponent.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "TarkovCopy/Player/Components/PlayerStatusComponent.h"
#include "TarkovCopy/Player/Components/PlayerMovementComponent.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/Weapons/FlashGrenade.h"
#include "TarkovCopy/Weapons/BulletProjectile.h"
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
	PlayerStatusComponent->Init(playerController, this);
	PlayerMovementComponent->Init(playerController, this,PlayerStatusComponent->DefaultSprintingSpeed
		, PlayerStatusComponent->DefaultWalkingSpeed
		, PlayerStatusComponent->DefaultAdsWalkingSpeed);

	//TODO:나중에 인벤토리 초기화 고칠것
	inventory = nullptr;
	inventory = NewObject<UInventory>(GetWorld(),inventoryOrigin);
	inventory->Init(this);


	TArray<UItemInfo*> tempContainers = JsonSaveAndLoader::LoadBackpackItemContainers(GetWorld());
	if (tempContainers.Num() > 0)
	{
		for (int i = 0; i < tempContainers.Num(); i++)
		{
			if (tempContainers[i])
			{
				inventory->AddNewItemToInventory(tempContainers[i]);
				playerController->AddItem(inventory->GetBackpack()->GetItemContainers()[inventory->GetBackpack()->GetItemContainers().Num()-1], inventory);
			}
		}
	}

	if (playerController != nullptr)
		playerController->InitInvenotry();
	//Pool initialize;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	PlayerMovementComponent->Loop();
	
	if (isFirePressed)
	{
		if (currentActiveGun && currentActiveGun->CanFireWeapon() && !IsCloseToWall()) // 세미에서 오토로 바꿔서 쏠때 발생하는 버그를 막기 위하여 처리
		{
			if (currentActiveGun->isAutoFire && !isFired)
			{
				ActualFireWeapon();
			}
			else if (!isFired)
			{
				isFired = true;
				ActualFireWeapon();
			}
		}
		else if (currentActiveGun && (!IsFiring()) && (!IsReloading()) && IsEmptyMagazine() && !IsCloseToWall())
		{
			if (!isFired)
			{
				isFired = true;
				currentActiveGun->EmptyFireWeapon();
			}
		}
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
					if (inventory->GetEquippedHelmet() != nullptr &&
						inventory->GetEquippedHelmet()->curDurability >= inventory->GetEquippedHelmet()->damageDecreaseAmount)
					{
						Damage -= inventory->GetEquippedHelmet()->damageDecreaseAmount;
						inventory->GetEquippedHelmet()->curDurability -= inventory->GetEquippedHelmet()->damageDecreaseAmount;
					}
				}
			}

			PlayerStatusComponent->DecreaseHealth(Damage);
			playerController->ShowHitIndicator(instigatorPawn->GetActorLocation());
		}



	return Damage;
}


bool APlayerCharacter::PickupItem(UItemInfo* pItemInfo)
{
	bool isItemAdded = inventory->AddNewItemToInventory(pItemInfo);
	if (isItemAdded && playerController != nullptr)
	{
		playerController->AddItem(pItemInfo,inventory);
	}

	return isItemAdded;
}

void APlayerCharacter::StartMoveItemPos(UItemInfo* pItemInfo)
{
	inventory->StartMoveItemPos(pItemInfo);
}

bool APlayerCharacter::CanItemMoveTo(FSlateRect pIntSlateRect)
{
	return inventory->CanItemMoveTo(pIntSlateRect);
}

void APlayerCharacter::MoveItemTo(UItemInfo* pItemInfo, FSlateRect pIntSlateRect)
{
	inventory->MoveItemTo(pItemInfo, pIntSlateRect);
}

void APlayerCharacter::FailedToMoveItemPos(UItemInfo* pItemInfo)
{
	inventory->FailedToMoveItemPos(pItemInfo);
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
	if (primaryWeapon != nullptr)
		return;
	primaryWeapon = GetWorld()->SpawnActor<ABaseGun>(pWeaponOrigin);
	if (primaryWeapon != nullptr)
	{
		primaryWeapon->SetInfo(pItemWeapon);
		primaryWeapon->SetParentMeshFPP(GetMesh());
		primaryWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		primaryWeapon->SetOwner(this);
		primaryWeapon->weaponOwnerCharacter = this;
	}
	inventory->SetPrimaryWeaponItem(pItemWeapon);
	EquipPrimary();
}

void APlayerCharacter::AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin, UItemWeapon* pItemWeapon)
{
	if (secondaryWeapon != nullptr)
		return;
	secondaryWeapon = GetWorld()->SpawnActor<ABaseGun>(m9Origin);
	if (secondaryWeapon != nullptr)
	{
		secondaryWeapon->SetInfo(pItemWeapon);
		secondaryWeapon->SetParentMeshFPP(GetMesh());
		secondaryWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		secondaryWeapon->SetOwner(this);
		secondaryWeapon->weaponOwnerCharacter = this;
	}

	inventory->SetSecondaryWeaponItem(pItemWeapon);
	EquipSecondary();
}

void APlayerCharacter::RemovePrimary()
{
	if (primaryWeapon == currentActiveGun)
		currentActiveGun = nullptr;
	if(primaryWeapon)
		primaryWeapon->Destroy();
	primaryWeapon = nullptr;

	if (secondaryWeapon)
	{
		EquipSecondary();
	}
}

void APlayerCharacter::RemoveSecondary()
{
	if (secondaryWeapon == currentActiveGun)
		currentActiveGun = nullptr;
	if (secondaryWeapon)
		secondaryWeapon->Destroy();
	secondaryWeapon = nullptr;

	if (primaryWeapon)
	{
		EquipPrimary();
	}
}


void APlayerCharacter::FireProjectile(float pDamage, float pVelocity, float pMass, FVector pFireStartPos, FVector pShootDir)
{
	bool isProjectileFired = false;
	for (int i = 0; i < bulletProjectilePools.Num(); i++)
	{
		if (bulletProjectilePools[i] && !bulletProjectilePools[i]->IsFired())
		{
			bulletProjectilePools[i]->ReactivateProjectile(pDamage, pVelocity, pMass, Cast<APawn>(this), pShootDir);
			bulletProjectilePools[i]->LaunchProjectile();
			bulletProjectilePools[i]->SetActorLocation(pFireStartPos);
			isProjectileFired = true;
			break;
		}
	}


	if (!isProjectileFired)
	{
		ABulletProjectile* bullet = GetWorld()->SpawnActor<ABulletProjectile>(bulletProjectileOrigin, pFireStartPos, pShootDir.Rotation());
		bullet->SetInstigator(this);
		bullet->SetOwner(this);
		bulletProjectilePools.Add(bullet);
		//총알의 주인과 선동자 설정 - 주인을 나중에 무기로 바꿀려면 풀을 무기에서 생성하게끔 기능 자체를 옮겨줘야한다. - 특히 멀티 플레이어에서 잘 지켜보도록.
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->ReactivateProjectile(pDamage, pVelocity, pMass,  Cast<APawn>(this), pShootDir);
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->LaunchProjectile();
		bulletProjectilePools[bulletProjectilePools.Num() - 1]->SetActorLocation(pFireStartPos);
		isProjectileFired = true;
	}
}

void APlayerCharacter::AddHelmet(UItemHelmet* pHelmetInfo)
{
	//TODO: 메쉬 찾아서 켜줘
	helmetMesh->SetVisibility(true);
	inventory->EquipHelmet(pHelmetInfo);
}

void APlayerCharacter::RemoveHelmet(UItemHelmet* pHelmetInfo)
{
	helmetMesh->SetVisibility(false);
	inventory->UnequipHelmet();
}

void APlayerCharacter::SaveEquipments()
{
	inventory->SaveEquipments();
}

bool APlayerCharacter::IsWeaponEquiped()
{
	return (currentActiveGun != nullptr);
}

bool APlayerCharacter::IsAds()
{
	return (currentActiveGun && currentActiveGun->isAds);
}

bool APlayerCharacter::IsWalking()
{
	return (PlayerMovementComponent && !PlayerMovementComponent->IsSprinting && GetVelocity().Size()>0) ;
}

int APlayerCharacter::GetWeaponCode()
{
	return (currentActiveGun)?currentActiveGun->itemCode:-1;
}

bool APlayerCharacter::IsEmptyMagazine()
{
	return (currentActiveGun && currentActiveGun->curMagRounds <= 0);
}

bool APlayerCharacter::IsCloseToWall()
{
	return isCloseToWall;
}

bool APlayerCharacter::IsShotgun()
{
	return (currentActiveGun && currentActiveGun->itemCode == 3);
}

int APlayerCharacter::GetReloadState()
{
	return (currentActiveGun && currentActiveGun->itemCode == 3)?currentActiveGun->reloadState : -1;
}

bool APlayerCharacter::IsSprinting()
{
	return (PlayerMovementComponent && PlayerMovementComponent->IsSprinting && GetVelocity().Size() > 0);
}

bool APlayerCharacter::IsFiring()
{
	return (currentActiveGun && currentActiveGun->isFiring);
}

bool APlayerCharacter::IsReloading()
{
	return (currentActiveGun && currentActiveGun->isReloading);
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
	if (primaryWeapon)
	{
		primaryWeapon->SetActorHiddenInGame(false);
		primaryWeapon->SetActorTickEnabled(true);
		currentActiveGun = primaryWeapon;
		currentActiveGun->EquipWeapon();
		if (secondaryWeapon)
		{
			secondaryWeapon->SetActorHiddenInGame(true);
			secondaryWeapon->SetActorTickEnabled(false);
		}
	}
}

void APlayerCharacter::EquipSecondary()
{
	if (secondaryWeapon)
	{
		secondaryWeapon->SetActorHiddenInGame(false);
		secondaryWeapon->SetActorTickEnabled(true);
		currentActiveGun = secondaryWeapon;
		currentActiveGun->EquipWeapon();
		if (primaryWeapon)
		{
			primaryWeapon->SetActorHiddenInGame(true);
			primaryWeapon->SetActorTickEnabled(false);
		}
	}
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
	FVector start;
	FRotator dir;

	currentActiveGun->UpdateMuzzleInfo();
	start = currentActiveGun->muzzleStart;
	if (IsAds())
	{
		dir = currentActiveGun->muzzleDir;
	}
	else
	{
		FVector viewpointPos;
		FRotator viewpointRotation;
		GetController()->GetPlayerViewPoint(viewpointPos, viewpointRotation);
		dir = ((viewpointPos +(viewpointRotation.Vector()*500.f)) - start).Rotation();
	}

	currentActiveGun->FireWeapon(start, dir);

	if (IsAds())
	{
		AddControllerPitchInput(-currentActiveGun->adsBulletVerticalSpreadIncrement);
		AddControllerYawInput(currentActiveGun->adsBulletHorizontalSpreadIncrement);
	}

}

void APlayerCharacter::FireUpWeapon()
{
	isFirePressed = false;
	isFired = false;
}

void APlayerCharacter::SetADSWeapon()
{
	if (playerController->isInventoryOpened )
	{
		return;
	}

	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;

	if (currentActiveGun && !IsCloseToWall())
	{
		currentActiveGun->SetADS();
		playerController->SetADS();
	}
}

void APlayerCharacter::SetHipfireWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;
	if (currentActiveGun)
	{
		currentActiveGun->SetHipfire();
		playerController->SetHipfire();
	}
}

void APlayerCharacter::ReloadWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;


	if (currentActiveGun && !currentActiveGun->isReloading && !inventory->isUsingInventory)
	{
		int needAmmo = currentActiveGun->maximumMagRounds - currentActiveGun->curMagRounds;
		

		//TODO: 체크 로직은 인벤토리가 추가되면 바로 바뀔 것
		//needAmmo가 현재 보유 수보다 같거나 적고 현재 발사가 가능한 상태이면서 재장전 중이 아니면 재장전 아니면 빠꾸
		if (needAmmo > 0)
		{
			int ownedAmmo = 0;
			inventory->isUsingInventory = true;
			if (currentActiveGun == primaryWeapon)
			{
				//TODO: 실기능 구현
				ownedAmmo = inventory->GetAllPrimaryWeaponAmmo(currentActiveGun->GetClass()->GetName());
			}
			else if (currentActiveGun == secondaryWeapon)
			{
				ownedAmmo = inventory->GetAllSecondaryWeaponAmmo(currentActiveGun->GetClass()->GetName());
			}

			if (ownedAmmo == 0)
			{
				inventory->isUsingInventory = false;
				return;
			}

			if (needAmmo > ownedAmmo)
			{
				needAmmo = ownedAmmo;
			}

			currentActiveGun->Reload(needAmmo);
			if (currentActiveGun == primaryWeapon)
			{
				inventory->UsePrimaryWeaponAmmo(needAmmo,currentActiveGun->GetClass()->GetName());
			}
			else
			{
				inventory->UseSecondaryWeaponAmmo(needAmmo, currentActiveGun->GetClass()->GetName());
			}

			inventory->UpdateAndCleanupBackpack();
			playerController->UpdateInventoryUI();

			inventory->isUsingInventory = false;
		}
	}
}

void APlayerCharacter::SetWeaponSelector()
{
	if (currentActiveGun && currentActiveGun->isCanAutoFire)
	{
		currentActiveGun->isAutoFire = !currentActiveGun->isAutoFire;
	}
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

	if (currentActiveGun && !IsReloading() && !IsSprinting() && !IsAds() && !IsFiring())
	{
		currentActiveGun->InspectWeapon();
	}
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
	UItemInfo* itemReference = inventory->HasItemType(ItemType::GRENADE);
	if (itemReference == nullptr)
	{
		return;
	}

	bool isThrowing = false;
	for (int i = 0; i < handGrenadePools.Num(); i++)
	{
		if (handGrenadePools[i] &&!handGrenadePools[i]->IsActive())
		{
			//Add Physics power
			handGrenadePools[i]->ThrowGrenade(GetActorForwardVector(),GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * 100.f);
			inventory->UseItem(itemReference);
			inventory->UpdateAndCleanupBackpack();
			playerController->UpdateInventoryUI();
			isThrowing = true;
			break;
		}
	}

	if (!isThrowing)
	{
		AHandGrenade* hand = GetWorld()->SpawnActor<AHandGrenade>(handGrenadeOrigin);
		hand->SetInstigator(this);
		hand->SetOwner(this);
		handGrenadePools.Add(hand);

		handGrenadePools[handGrenadePools.Num() - 1]->ReactivateGrenade();
		//Add Physics power
		handGrenadePools[handGrenadePools.Num() - 1]->SetActorLocation(GetActorLocation());
		handGrenadePools[handGrenadePools.Num() - 1]->ThrowGrenade(GetActorForwardVector(), GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * 100.f);

		inventory->UseItem(itemReference);
		inventory->UpdateAndCleanupBackpack();
		playerController->UpdateInventoryUI();

		isThrowing = true;
	}
}

void APlayerCharacter::ThrowFlashGrenade()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped || gameMode->isPauseMenuOpened))
		return;
	UItemInfo* itemReference = inventory->HasItemType(ItemType::FLASHGRENADE);
	if (itemReference == nullptr)
	{
		return;
	}

	bool isThrowing = false;

	for (int i = 0; i < flashGrenadePools.Num(); i++)
	{
		if (flashGrenadePools[i] && !flashGrenadePools[i]->IsActive())
		{
			//Add Physics power
			flashGrenadePools[i]->ThrowGrenade(GetActorForwardVector(), GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * 100.f);

			inventory->UseItem(itemReference);
			inventory->UpdateAndCleanupBackpack();
			playerController->UpdateInventoryUI();
			isThrowing = true;
			break;
		}
	}

	if (!isThrowing)
	{
		AFlashGrenade* flash = GetWorld()->SpawnActor<AFlashGrenade>(flashGrenadeOrigin);
	
		flashGrenadePools.Add(flash);
		flashGrenadePools[flashGrenadePools.Num() - 1]->SetInstigator(this);
		flashGrenadePools[flashGrenadePools.Num() - 1]->SetOwner(this);
		//Add Physics power
		flashGrenadePools[flashGrenadePools.Num() - 1]->ThrowGrenade(GetActorForwardVector(), GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * 100.f);

		inventory->UseItem(itemReference);
		inventory->UpdateAndCleanupBackpack();
		playerController->UpdateInventoryUI();
		isThrowing = true;
	}
}


void APlayerCharacter::HealPlayer(float pHealAmount)
{
	PlayerStatusComponent->HealPlayer(pHealAmount);
}