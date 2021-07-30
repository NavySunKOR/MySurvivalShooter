// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDWidget.h"

FTimerManager& UInGameHUDWidget::GetWorldTimer()
{
	return GetWorld()->GetTimerManager();
}


void UInGameHUDWidget::FlashProcessing()
{
	CurFlashTime += PROCESS_FRAMERATE;
	//���� ���� �ð��� ���� �̻��� �Ѿ���� ���� �ð� ���� ����Ͽ� fade
	if (CurFlashTime > FlashFadeStartTime)
	{
		FlashSplash->SetRenderOpacity(FlashSplash->GetRenderOpacity()-FlashFadeAmount);
	}

	if (CurFlashTime > FlashInterval)
	{
		CurFlashTime = 0.f;
		FlashInterval = 0.f;
		FlashFadeAmount = 0.f;
		FlashFadeStartTime = 0.f;
		FlashSplash->SetRenderOpacity(0.f);
		GetWorldTimer().ClearTimer(FlashTimerHandle);
	}
}

void UInGameHUDWidget::IndicatorProcessing()
{
	CurHitIndicatorTime += PROCESS_FRAMERATE;

	FVector characterForward = GetOwningPlayerPawn()->GetActorForwardVector();
	FVector hitDir = (HitFromPos - GetOwningPlayerPawn()->GetActorLocation()).GetSafeNormal();
	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(characterForward, hitDir)));
	float crossAngle = FVector::DotProduct(FVector::CrossProduct(characterForward, hitDir), FVector::UpVector);
	
	if (crossAngle < 0)
	{
		angle = -angle;
	}
	
	
	if (angle > 180.f)
	{
		angle -= 180.f;
	}
	HitIndicate->SetRenderTransformAngle(angle);
	
	if (CurHitIndicatorTime > HitIndicatorFadeStartTime)
	{
		HitIndicate->SetRenderOpacity(HitIndicate->GetRenderOpacity()- HitIndicatorFadeAmount);
	}
	
	if (CurHitIndicatorTime > HitIndicatorInterval)
	{
		CurHitIndicatorTime = 0.f;
		HitIndicate->SetRenderOpacity(0.f); 
		GetWorldTimer().ClearTimer(HitIndicatorTimerHandle);
	}
}

void UInGameHUDWidget::UpdateHealthHud(float pCurHealth)
{
	DamageSplash->SetRenderOpacity(1.f - (pCurHealth / 100.f));
}

void UInGameHUDWidget::ShowHitIndicator(FVector pHitDir)
{
	HitFromPos = pHitDir;
	CurHitIndicatorTime = 0.f;
	HitIndicatorFadeStartTime = HitIndicatorInterval * 0.75f;
	HitIndicatorFadeAmount = 1 / ((HitIndicatorInterval - FlashFadeStartTime) / PROCESS_FRAMERATE);
	HitIndicate->SetRenderOpacity(1.f);
	GetWorldTimer().ClearTimer(HitIndicatorTimerHandle);
	GetWorldTimer().SetTimer(HitIndicatorTimerHandle,this,&UInGameHUDWidget::IndicatorProcessing,PROCESS_FRAMERATE,true);
}

void UInGameHUDWidget::GetFlashed(float pFlashTime, FVector pFlashbangPos)
{
	APawn* player = GetOwningPlayerPawn();
	FVector dirToFlashbang = (pFlashbangPos - player->GetActorLocation()).GetSafeNormal();
	FVector playerForward = player->GetActorForwardVector();

	float vectorResult = FVector::DotProduct(dirToFlashbang, playerForward);
	//TODO: ����ź�� ����޴� �þ߰��� ���߿� CONSTANT�� ���Ƽ� ������ ��.
	//TODO: 0.6�̸� COS�ϸ� 50�� ������ �� - ������ ����ϸ� ������ �ٲ��ִ� ����� �߻��ϱ� ������, �׳� COS() ������ ���.
	if (vectorResult > 0.6f)
	{
		FlashInterval = pFlashTime;
		FlashFadeStartTime = FlashInterval / 2.f;

		/*
			����ź�� ���̵� �ƿ� �ϴµ� �ɸ��� �ð� = (����ź���ӽð� - ����ź ���� �ð�);
			����ź�� ���̵� �ƿ� �� �� ���� �����ϴ� Ƚ�� = ����ź�� ���̵� �ƿ� �ϴµ� �ɸ��� �ð� /ó�� �����ӽð� 
			1���� 0���� ���µ� �����Ӵ� ���ҷ� = 1 / ����ź�� ���̵� �ƿ� �� �� ���� �����ϴ� Ƚ��
		*/
		FlashFadeAmount =  1 /((FlashInterval - FlashFadeStartTime) / PROCESS_FRAMERATE);
		CurFlashTime = 0.f;
		FlashSplash->SetRenderOpacity(1.f);
		GetWorldTimer().ClearTimer(FlashTimerHandle);
		GetWorldTimer().SetTimer(FlashTimerHandle,this,&UInGameHUDWidget::FlashProcessing, PROCESS_FRAMERATE, true);
	}
}

void UInGameHUDWidget::SetCrosshairVisible()
{
	Crosshair->SetVisibility(ESlateVisibility::Visible);
}

void UInGameHUDWidget::SetCrosshairInvisible()
{
	Crosshair->SetVisibility(ESlateVisibility::Hidden);
}


/*
	���� �ڵ� - ��Ʈ������ �Ұ���.
	������ AFPPlayerController�� �ڵ�� ��°�� �����ߴ� ���� �ۼ�.
*/

//void AFPPlayerController::Tick()
//{
//	if (isHit)
//	{
//		hitIndicatorTimer += DeltaTime;
//
//		//���� ����
//
//		FVector characterForward = ownerPlayerCharacter->GetActorForwardVector();
//		FVector hitDir = (hitFromPos - ownerPlayerCharacter->GetActorLocation()).GetSafeNormal();
//		float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(characterForward, hitDir)));
//		float crossAngle = FVector::DotProduct(FVector::CrossProduct(characterForward, hitDir), FVector::UpVector);
//
//		if (crossAngle < 0)
//		{
//			angle = -angle;
//		}
//
//
//		if (angle > 180.f)
//		{
//			angle -= 180.f;
//		}
//		hitIndicatorActualUI->SetRenderTransformAngle(angle);
//
//		if (hitIndicatorTimer > hitIndicatorInterval / 2.f)
//		{
//			hitIndicatorFadeAmount = ((hitIndicatorInterval - hitIndicatorTimer) / (hitIndicatorInterval * 2.f));
//			hitIndicatorActualUI->SetRenderOpacity(hitIndicatorFadeAmount);
//		}
//
//		if (hitIndicatorTimer > hitIndicatorInterval)
//		{
//			isHit = false;
//			hitIndicatorTimer = 0.f;
//			hitIndicatorActualUI->SetRenderOpacity(0.f);
//		}
//	}
// 
// if (isFlashed)
//{
//	flashTimer += DeltaTime;
//	if (flashTimer > flashInterval / 2.f)
//	{
//		flashFadeAmount = ((flashInterval - flashTimer) / (flashInterval * 2.f));
//		flashHudBgUI->SetRenderOpacity(flashFadeAmount);
//	}

//	if (flashTimer > flashInterval)
//	{
//		isFlashed = false;
//		flashTimer = 0.f;
//		flashInterval = 0.f;
//		flashHudBgUI->SetRenderOpacity(0.f);
//	}
//}
//}

//void AFPPlayerController::UpdateHealthHud(float pCurHealth)
//{
//	float opacityAmount = 1.f - pCurHealth / 100.f;
//	if (opacityAmount < 0.2f)
//		opacityAmount = 0.2f;
//	healthHudBgUI->SetRenderOpacity(opacityAmount);
//}

//void AFPPlayerController::ShowHitIndicator(FVector pHitFrom)
//{
//	hitFromPos = pHitFrom;
//	isHit = true;
//	hitIndicatorTimer = 0.f;
//	hitIndicatorActualUI->SetRenderOpacity(1.f);
//
//}


//void AFPPlayerController::GetFlashed(float pFlashTime, FVector pFlashbangPos)
//{
//	FVector dirToFlashbang = (pFlashbangPos - ownerPlayerCharacter->GetActorLocation()).GetSafeNormal();
//	FVector playerForward = ownerPlayerCharacter->GetActorForwardVector();
//
//	float vectorResult = FVector::DotProduct(dirToFlashbang, playerForward);
//	float angle = FMath::RadiansToDegrees(FMath::Acos(vectorResult));
//	UE_LOG(LogTemp, Warning, TEXT("Value :%f"), angle)
//
//		//TODO: ����ź�� ����޴� �þ߰��� ���߿� CONSTANT�� ���Ƽ� ������ ��.
//		//TODO: 0.6�̸� COS�ϸ� 50�� ������ �� - ������ ����ϸ� ������ �ٲ��ִ� ����� �߻��ϱ� ������, �׳� COS() ������ ���.
//		if (vectorResult > 0.6f)
//		{
//			flashInterval = pFlashTime;
//			flashTimer = 0.f;
//			isFlashed = true;
//			//TODO: ����ź ��ũ�� ȿ��
//			flashHudBgUI->SetRenderOpacity(1.f);
//		}
//
//}