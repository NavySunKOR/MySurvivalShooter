// Copyright Epic Games, Inc. All Rights Reserved.


#include "TarkovCopyGameModeBase.h"

void ATarkovCopyGameModeBase::PlayerDied()
{
	isPlayerDied = true;
}

void ATarkovCopyGameModeBase :: QuestCompleted(AInteractableObject* questItem)
{
}

void ATarkovCopyGameModeBase::ExfilCompleted()
{
	isPlayerEscaped = true;
}

void ATarkovCopyGameModeBase::TryExfil()
{
}

void ATarkovCopyGameModeBase::CancelExfil()
{

}
