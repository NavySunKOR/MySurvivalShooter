// Fill out your copyright notice in the Description page of Project Settings.

#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "ItemM9.h"

bool UItemM9::Use()
{
	return false;
}
void UItemM9::DropItem(AFPPlayerController* pPlayerCon)
{
	pPlayerCon->RemoveSecondary();
}