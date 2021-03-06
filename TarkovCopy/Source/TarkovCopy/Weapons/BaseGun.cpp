// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemWeapon.h"
#include "TarkovCopy/Player/Character/PlayerCharacter.h"
#include "TarkovCopy/AI/Character/AICharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Components/ArrowComponent.h>

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	calcBulletSpreadRadius = initBulletSpreadRadius;
	fireInterval = 60.f / rpm;
	//curMagRounds = maximumMagRounds;
}

void ABaseGun::FireWeapon(FVector start, FRotator dir)
{
	curMagRounds--;
	isFiring = true;
	calcBulletSpreadRadius += bulletSpreadIncrement;
	UGameplayStatics::SpawnSoundAttached(fireWeaponSound, weaponComponents);
	UGameplayStatics::SpawnEmitterAttached(muzzleFireParticle, muzzleArrow);

	itemWeapon->currentMagazineAmmo = curMagRounds;
}

void ABaseGun::EmptyFireWeapon()
{
	isFiring = true;
	UGameplayStatics::SpawnSoundAttached(emptyFireWeaponSound, weaponComponents);
	if(weaponOwnerCharacter)
		weaponOwnerCharacter->PlayAnimMontage(emptyFireAnim);
	else
		weaponOwnerAICharacter->PlayAnimMontage(emptyFireAnim);

	if (isAds)
	{
		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(aimEmptyFireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(aimEmptyFireAnim);
	}
	else
	{
		if (weaponOwnerCharacter)
			weaponOwnerCharacter->PlayAnimMontage(emptyFireAnim);
		else
			weaponOwnerAICharacter->PlayAnimMontage(aimEmptyFireAnim);
	}
}

void ABaseGun::Reload(int pInsertMagazine)
{
	isReloading = true;
	tempInsertMag = pInsertMagazine;

}

void ABaseGun::SetADS()
{
	isAds = true;
}

void ABaseGun::SetHipfire()
{
	isAds = false;
}

void ABaseGun::SetParentMeshFPP(USkeletalMeshComponent* pMeshComp)
{
	parentMesh = pMeshComp;
	isFPP = true;
	magazineComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Magazine_Components")));
	attachmentComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Attachment_Components")));
	scopeComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Scope_Components")));
	weaponComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Weapon_Components")));
	sliderComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Slider_Components")));
	hammerComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Hammer_Components")));
	muzzleArrow = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("Muzzle_Flash_Position")));

	if (magazineComponents != nullptr)
		magazineComponents->AttachToComponent(parentMesh,FAttachmentTransformRules::KeepRelativeTransform , TEXT("Mag_Position"));

	if (attachmentComponents != nullptr)
		attachmentComponents->AttachToComponent(parentMesh, FAttachmentTransformRules::KeepRelativeTransform,TEXT("Weapon_Position"));

	if (scopeComponents != nullptr)
		scopeComponents->AttachToComponent(parentMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Position"));

	if(weaponComponents != nullptr)
		weaponComponents->AttachToComponent(parentMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Position"));

	if (sliderComponents != nullptr)
		sliderComponents->AttachToComponent(parentMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Slider_Position"));

	if (hammerComponents != nullptr)
		hammerComponents->AttachToComponent(parentMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Hammer_Position"));

}

void ABaseGun::SetParentMeshTPP(USkeletalMeshComponent* pMeshComp)
{
	parentMesh = pMeshComp;
	parentMesh->SetAnimInstanceClass(tppAnimBlueprints);

	SetInfo(NewObject<UItemWeapon>(this, weaponInfoForThirdPerson));

	weaponComponents = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Weapon_Root")));
	muzzleArrow = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("Muzzle_Arrow")));

	if (muzzleArrow)
	{
		muzzleStart = muzzleArrow->GetComponentLocation();
		muzzleDir = muzzleArrow->GetForwardVector().Rotation();
	}
}

void ABaseGun::EquipWeapon()
{
	parentMesh->SetAnimInstanceClass(fppAnimBlueprints);
}

void ABaseGun::InspectWeapon()
{
	weaponOwnerCharacter->PlayAnimMontage(inspectWeaponAnim);
}

void ABaseGun::SetInfo(UItemWeapon* pItemWeapon)
{
	itemWeapon = pItemWeapon;
	curMagRounds = itemWeapon->currentMagazineAmmo;
}

void ABaseGun::UpdateMuzzleInfo()
{
	if (muzzleArrow)
	{
		muzzleStart = muzzleArrow->GetComponentLocation();
		muzzleDir = muzzleArrow->GetForwardVector().Rotation();
	}
}

//오버라이드 가능성 : 샷건(한발씩 장전되는데 바로 쏴야하므로
bool ABaseGun::CanFireWeapon()
{
	return (!isFiring && !isReloading) && curMagRounds > 0;
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (muzzleArrow)
	{
		muzzleStart = muzzleArrow->GetComponentLocation();
		muzzleDir = muzzleArrow->GetForwardVector().Rotation();
	}

	calcBulletSpreadRadius -= bulletSpreadDecrementPerSecond;

	if (calcBulletSpreadRadius > maxBulletSpreadRadius || calcBulletSpreadRadius < initBulletSpreadRadius)
	{
		calcBulletSpreadRadius = FMath::Clamp<float>(calcBulletSpreadRadius, initBulletSpreadRadius, maxBulletSpreadRadius);
	}

}

// Called to bind functionality to input
void ABaseGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

bool ABaseGun::IsReloading()
{
	return isReloading;
}

bool ABaseGun::IsFiring()
{
	return isFiring;
}


