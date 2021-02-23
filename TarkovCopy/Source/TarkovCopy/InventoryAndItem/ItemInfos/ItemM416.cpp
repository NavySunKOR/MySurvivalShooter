// Fill out your copyright notice in the Description page of Project Settings.


#include "TarkovCopy/Player/Controller/FPPlayerController.h"
#include "ItemM416.h"

bool UItemM416::Use()
{
	return false;
}
void UItemM416::DropItem(AFPPlayerController* pPlayerCon)
{
	//DropFromPrimary
	pPlayerCon->RemovePrimary();

}