// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestItem.h"
#include <TarkovCopy/GameMode/EscapeGameMode.h>

void AQuestItem::Interact()
{
	Super::Interact();
	GetWorld()->GetAuthGameMode<AEscapeGameMode>()->QuestCompleted(this);
	Destroy();
}