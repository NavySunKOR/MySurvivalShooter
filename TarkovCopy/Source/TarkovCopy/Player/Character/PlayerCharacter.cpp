// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/Interactable/InteractableObject.h"
#include "TarkovCopy/Interactable/InteractableComponent.h"
#include "TarkovCopy/GameMode/TarkovCopyGameModeBase.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	gameMode =Cast<ATarkovCopyGameModeBase>(GetWorld()->GetAuthGameMode());
	playerController = Cast<AFPPlayerController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;
	curHp = maxHp;

	//TODO:나중에 인벤토리 초기화 고칠것

	inventory = nullptr;
	inventory = NewObject<UInventory>(this,inventoryOrigin);
	inventory->Init(this);

	if (playerController != nullptr)
		playerController->InitInvenotry();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkCloseToWallTimer += DeltaTime;
	if (checkCloseToWallTimer > checkCloseToWallInterval)
	{
		checkCloseToWallTimer = 0.f;
		CheckCloseToWall();
	}

	//Movement

	if (moveVerticalValue * moveVerticalValue > 0 || moveHorizontalValue * moveHorizontalValue > 0)
	{
		if (moveVerticalValue == 0 && IsSprinting())
		{
			SetWalking();
		}

		if (moveVerticalValue > 0)
		{
			if (moveHorizontalValue == 0 || (moveVerticalValue > 0 && moveHorizontalValue * moveHorizontalValue > 0))
				maxWalkValue = (IsSprinting()) ? sprintingSpeed : walkingSpeed;
			else
				maxWalkValue = walkingSpeed;

			maxWalkValue = (IsAds()) ? adsWalkingSpeed : maxWalkValue;
		}
		else
		{
			maxWalkValue = walkingSpeed;
			maxWalkValue = (IsAds()) ? adsWalkingSpeed : maxWalkValue;
		}

		GetCharacterMovement()->MaxWalkSpeed = maxWalkValue;
		AddMovementInput((GetActorForwardVector() * moveVerticalValue + GetActorRightVector() * moveHorizontalValue)/1.4f);
	}


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetSprinting);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SetWalking);
	PlayerInputComponent->BindAction(TEXT("EquipPrimary"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipPrimary);
	PlayerInputComponent->BindAction(TEXT("EquipSecondary"), EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipSecondary);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetADSWeapon);
	PlayerInputComponent->BindAction(TEXT("ADS"), EInputEvent::IE_Released, this, &APlayerCharacter::SetHipfireWeapon);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &APlayerCharacter::SetStanding);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Inventory);
	PlayerInputComponent->BindAction(TEXT("InspectWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InspectWeapon);

	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &APlayerCharacter::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &APlayerCharacter::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &APlayerCharacter::RotateVertical);
}

void APlayerCharacter::CheckCloseToWall()
{
	FVector startPos;
	FVector dir;
	FHitResult hit;
	startPos = GetActorLocation();
	dir = GetActorForwardVector();
	FCollisionQueryParams paramCol;
	paramCol.AddIgnoredActor(this);
	if (currentActiveGun)
		paramCol.AddIgnoredActor(currentActiveGun);

	if (GetWorld()->LineTraceSingleByChannel(hit, startPos, startPos + dir * 150.f, ECollisionChannel::ECC_Pawn, paramCol))
	{
		isCloseToWall = true;
	}
	else
	{
		isCloseToWall = false;
	}
}

//void APlayerCharacter::BeginDestroy()
//{
//	//if (playerController)
//	//	playerController->ConditionalBeginDestroy();
//	//if (currentActiveGun)
//	//	currentActiveGun->ConditionalBeginDestroy();
//	//if (primaryWeapon)
//	//	primaryWeapon->ConditionalBeginDestroy();
//	//if (secondaryWeapon)
//	//	secondaryWeapon->ConditionalBeginDestroy();
//}

float APlayerCharacter::HealPlayer(float pHealAmount)
{
	curHp += pHealAmount;
	UE_LOG(LogTemp, Warning, TEXT("Healing"));
	if (curHp > maxHp)
	{
		curHp = maxHp;
	}
	return curHp;
}

void APlayerCharacter::TookDamage(float damage, FHitResult pHitParts)
{
	curHp -= damage;
	if (curHp <= 0)
	{
		curHp = 0.f;
		playerController->Dead();
	}
	playerController->UpdateHealthHud(curHp);
}

bool APlayerCharacter::PickupItem(UItemInfo* pItemInfo)
{
	bool isItemAdded = inventory->AddItemToInventory(pItemInfo);
	if (isItemAdded && playerController != nullptr)
	{
		playerController->AddItem(pItemInfo,inventory);
	}

	return isItemAdded;
}

void APlayerCharacter::AddPrimary(TSubclassOf<ABaseGun> pWeaponOrigin)
{
	primaryWeapon = GetWorld()->SpawnActor<ABaseGun>(pWeaponOrigin);
	if (primaryWeapon != nullptr)
	{
		primaryWeapon->SetParentMeshFPP(GetMesh());
		primaryWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		primaryWeapon->SetOwner(this);
		primaryWeapon->weaponOwnerCharacter = this;
	}
	EquipPrimary();
}

void APlayerCharacter::AddSecondary(TSubclassOf<ABaseGun> pWeaponOrigin)
{
	secondaryWeapon = GetWorld()->SpawnActor<ABaseGun>(m9Origin);
	if (secondaryWeapon != nullptr)
	{
		secondaryWeapon->SetParentMeshFPP(GetMesh());
		secondaryWeapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		secondaryWeapon->SetOwner(this);
		secondaryWeapon->weaponOwnerCharacter = this;
	}

	EquipSecondary();
}

void APlayerCharacter::RemovePrimary()
{
	if (currentActiveGun == primaryWeapon)
	{
		primaryWeapon->Destroy();
		currentActiveGun->Destroy();
		currentActiveGun = nullptr;
		primaryWeapon = nullptr;

		if (secondaryWeapon)
		{
			EquipSecondary();
		}
	}
}

void APlayerCharacter::RemoveSecondary()
{
	if (currentActiveGun == secondaryWeapon)
	{
		secondaryWeapon->Destroy();
		currentActiveGun->Destroy();
		currentActiveGun = nullptr;
		secondaryWeapon = nullptr;

		if (primaryWeapon)
		{
			EquipPrimary();
		}
	}
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
	return (!isSprinting && GetVelocity().Size()>0) ;
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
	return (isSprinting && GetVelocity().Size() > 0);
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
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
	{
		moveVerticalValue = 0;
		return;
	}

	moveVerticalValue = pValue;
}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
	{
		moveHorizontalValue = 0;
		return;
	}
	moveHorizontalValue = pValue;
}

void APlayerCharacter::RotateHorizontal(float pValue)
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;
	AddControllerYawInput(pValue);
}

void APlayerCharacter::RotateVertical(float pValue)
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;
	AddControllerPitchInput(-pValue);
}

void APlayerCharacter::SetSprinting()
{
	if (!isCrouch && ((moveVerticalValue > 0 && moveHorizontalValue == 0) || (moveVerticalValue > 0 && moveHorizontalValue * moveHorizontalValue> 0)))
	{
		isSprinting = true;
	}
}

void APlayerCharacter::SetWalking()
{
	isSprinting = false;
}

void APlayerCharacter::SetCrouch()
{
	if (IsSprinting())
	{
		SetWalking();
	}
	Crouch();
	//springArm->TargetOffset = originalSpringArmPos - FVector(0, 0, 500.f);
	isCrouch = true;
}

void APlayerCharacter::SetStanding()
{
	UnCrouch();
	//springArm->TargetOffset = originalSpringArmPos;
	isCrouch = false;
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
	if (playerController->isInventoryOpened || gameMode->isPlayerDied || gameMode->isPlayerEscaped)
	{
		return;
	}
	if (currentActiveGun && currentActiveGun->CanFireWeapon() && !IsCloseToWall())
	{
		FVector start;
		FRotator dir;
		if(!IsAds())
			GetController()->GetPlayerViewPoint(start, dir);
		else 
		{
			currentActiveGun->UpdateMuzzleInfo();
			start = currentActiveGun->muzzleStart;
			dir = currentActiveGun->muzzleDir;
		}

		currentActiveGun->FireWeapon(start,dir);
	}
	else if (currentActiveGun && (!IsFiring()) && (!IsReloading()) && IsEmptyMagazine() && !IsCloseToWall())
	{
		currentActiveGun->EmptyFireWeapon();
	}
}

void APlayerCharacter::SetADSWeapon()
{
	if (playerController->isInventoryOpened )
	{
		return;
	}

	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;

	if (currentActiveGun && !IsCloseToWall())
	{
		currentActiveGun->SetADS();
		playerController->SetADS();
	}
}

void APlayerCharacter::SetHipfireWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;
	if (currentActiveGun)
	{
		currentActiveGun->SetHipfire();
		playerController->SetHipfire();
	}
}

void APlayerCharacter::ReloadWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("isUsingInventory : %d"), inventory->isUsingInventory);
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
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

			UE_LOG(LogTemp, Warning, TEXT("Reload Start"));
			currentActiveGun->Reload(needAmmo);
			if (currentActiveGun == primaryWeapon)
			{
				UE_LOG(LogTemp, Warning, TEXT("Reloading"));
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

void APlayerCharacter::Interact()
{
	if (playerController->isInventoryOpened || gameMode->isPlayerDied || gameMode->isPlayerEscaped)
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
				interComp->Interact();
			}
		}
	}
}

void APlayerCharacter::InspectWeapon()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;

	if (currentActiveGun && !IsReloading() && !IsSprinting() && !IsAds() && !IsFiring())
	{
		currentActiveGun->InspectWeapon();
	}
}

void APlayerCharacter::Inventory()
{
	if (gameMode && (gameMode->isPlayerDied || gameMode->isPlayerEscaped))
		return;
	if (IsAds())
	{
		SetHipfireWeapon();
	}

	playerController->OpenCloseInventory();
}
