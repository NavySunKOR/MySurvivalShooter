/*
	author : ��ȣ��
	description : ĳ������ ���� ���� ü��,�̵� �ӵ�, ����, ������ �ǰ� ��


*/


#include "PlayerStatusComponent.h"
#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "TarkovCopy/UI/InGameHUD.h"

UPlayerStatusComponent::UPlayerStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatusComponent::InitStatusComponent(AFPPlayerController* pPlayerController, APlayerCharacter* pPlayerCharacter)
{
	Super::Init(pPlayerController, pPlayerCharacter);
	CurHp = MaxHp;
}

void UPlayerStatusComponent::HealPlayer(float pHealAmount)
{
	CurHp += pHealAmount;
	if (CurHp > MaxHp)
	{
		CurHp = MaxHp;
	}

	PlayerController->GetInGameHUD()->UpdateHealthHud(CurHp);
}

void UPlayerStatusComponent::DecreaseHealth(float pDamageAmount)
{
	CurHp -= pDamageAmount;
	if (CurHp <= 0 )
	{
		CurHp = 0.f;
		PlayerController->Dead();
	}

	PlayerController->GetInGameHUD()->UpdateHealthHud(CurHp);
}
