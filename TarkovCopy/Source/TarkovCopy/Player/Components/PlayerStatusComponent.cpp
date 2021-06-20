/*
	author : 조호연
	description : 캐릭터의 스탯 관련 체력,이동 속도, 방어력, 데미지 피격 등


*/


#include "PlayerStatusComponent.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"

UPlayerStatusComponent::UPlayerStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatusComponent::Init(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
{
	CurHp = MaxHp;
}

void UPlayerStatusComponent::HealPlayer(float pHealAmount)
{
	CurHp += pHealAmount;
	if (CurHp > MaxHp)
	{
		CurHp = MaxHp;
	}

	PlayerController->UpdateHealthHud(CurHp);
}

void UPlayerStatusComponent::DecreaseHealth(float pDamageAmount)
{
	CurHp -= pDamageAmount;
	if (CurHp <= 0 )
	{
		CurHp = 0.f;
		PlayerController->Dead();
	}

	PlayerController->UpdateHealthHud(CurHp);
}
