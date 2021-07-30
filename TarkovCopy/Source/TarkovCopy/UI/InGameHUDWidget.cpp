// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUDWidget.h"

FTimerManager& UInGameHUDWidget::GetWorldTimer()
{
	return GetWorld()->GetTimerManager();
}


void UInGameHUDWidget::FlashProcessing()
{
	CurFlashTime += PROCESS_FRAMERATE;
	//섬광 지속 시간의 절반 이상이 넘어갔을때 남은 시간 별로 계산하여 fade
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
	//TODO: 섬광탄에 영향받는 시야각을 나중에 CONSTANT로 몰아서 정의할 것.
	//TODO: 0.6이면 COS하면 50도 가까이 됨 - 각도로 계산하면 각도로 바꿔주는 비용이 발생하기 때문에, 그냥 COS() 값으로 계산.
	if (vectorResult > 0.6f)
	{
		FlashInterval = pFlashTime;
		FlashFadeStartTime = FlashInterval / 2.f;

		/*
			섬광탄이 페이드 아웃 하는데 걸리는 시간 = (섬광탄지속시간 - 섬광탄 시작 시간);
			섬광탄이 페이드 아웃 될 때 까지 감소하는 횟수 = 섬광탄이 페이드 아웃 하는데 걸리는 시간 /처리 프레임시간 
			1에서 0까지 가는데 프레임당 감소량 = 1 / 섬광탄이 페이드 아웃 될 때 까지 감소하는 횟수
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
	이전 코드 - 포트폴리오 소개용.
	이전에 AFPPlayerController에 코드로 통째로 구현했던 내용 작성.
*/

//void AFPPlayerController::Tick()
//{
//	if (isHit)
//	{
//		hitIndicatorTimer += DeltaTime;
//
//		//각도 적용
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
//		//TODO: 섬광탄에 영향받는 시야각을 나중에 CONSTANT로 몰아서 정의할 것.
//		//TODO: 0.6이면 COS하면 50도 가까이 됨 - 각도로 계산하면 각도로 바꿔주는 비용이 발생하기 때문에, 그냥 COS() 값으로 계산.
//		if (vectorResult > 0.6f)
//		{
//			flashInterval = pFlashTime;
//			flashTimer = 0.f;
//			isFlashed = true;
//			//TODO: 섬광탄 스크린 효과
//			flashHudBgUI->SetRenderOpacity(1.f);
//		}
//
//}