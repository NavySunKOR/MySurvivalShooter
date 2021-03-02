// Fill out your copyright notice in the Description page of Project Settings.

#include <Layout/SlateRect.h>
#include "Backpack.h"


void UBackpack::Init()
{
	invenVisualize = new bool* [capacityWidth];
	for (int i = 0; i < capacityWidth; i++)
	{
		invenVisualize[i] = new bool[capacityHeight];
	}

	for (int i = 0; i < capacityWidth; i++)
	{
		for (int j = 0; j < capacityHeight; j++)
		{
			invenVisualize[i][j] = false;

		}
	}

	//백팩에서 begin destroy를 정의하고 시도해보았으나, UObject isValid 관련 assertion 에러만 나서 제거 하고 다음과 같이 작성하였다.

	UE_LOG(LogTemp, Warning, TEXT("Die! %d"), itemContainers.Num());

	if (itemContainers.Num() > 0)
	{
		for (UItemInfo* item : itemContainers)
		{
			UE_LOG(LogTemp, Warning, TEXT("Die!"));
			itemContainers.Remove(item);
		}
	}
}

std::tuple<bool, int, int> UBackpack::HasEmptySpaceWidthAxis(UItemInfo* pItemInfo)
{
	int spaceXCount = 0;
	int spaceYCount = 0;
	int xIncrease = 1;

	for (int y = 0; y < capacityHeight; y++)
	{
		for (int x = 0; x < capacityWidth; x += xIncrease)
		{
			//한번 width 만큼 건너 뛰었으면 다시 리셋 시킨다.
			xIncrease = 1;

			//해당칸이 비었다면
				if (invenVisualize[x][y] == false)
				{
					//그 근처를 아이템의 width, height 만큼 탐색
					for (int startY = y; startY < y + pItemInfo->height; startY++)
					{
						for (int startX = x; startX < x + pItemInfo->width; startX++)
						{
							//startX가 인벤 크기를 벗어나면 공간 없음 처리
							if (startX >= capacityWidth)
							{
								spaceXCount = 0;
								break;
							}

							//해당 인벤에 만약에 빈칸이 하나라도 통째로 무쓸모.
							if (invenVisualize[startX][startY] == false)
							{
								spaceXCount++;
							}
							else
							{
								spaceXCount = 0;
								break;
							}
						}
						if (spaceXCount == 0)
						{
							spaceYCount = 0;
							break;
						}
						else
						{
							spaceYCount++;
						}
					}

					//해당 공간이 사용할 수 있는 공간이면 그자리에서 끝내고 아니면 계속 탐색
					if (spaceXCount >= pItemInfo->width && spaceYCount >= pItemInfo->height)
					{
						//TODO:tuple로  포지션도 같이 리턴할것.
						return  std::tuple<bool, int, int>(true, x, y);
					}
					else
					{
						//이미 width 만큼 해당 지점을 검색했기 때문에 다음 지점으로 패스 시킨다.
						xIncrease += pItemInfo->width;
					}
				}
		}
	}
	//이미 윗단계에서 체크를 못했다면 없다는 것이므로 빠져나감.
	return std::tuple<bool, int, int>(false, -1, -1);
}


std::tuple<bool, int, int> UBackpack::HasEmptySpaceHeightAxis(UItemInfo* pItemInfo)
{
	int spaceXCount = 0;
	int spaceYCount = 0;
	int yIncrease = 1;

	for (int x = 0; x < capacityWidth; x++)
	{
		for (int y = 0; y < capacityHeight; y += yIncrease)
		{
			//한번 height 만큼 건너 뛰었으면 다시 리셋 시킨다.
			yIncrease = 1;

			//해당칸이 비었다면
			if (invenVisualize[x][y] == false)
			{
				//그 근처를 아이템의 width, height 만큼 탐색
				for (int startX = x; startX < x + pItemInfo->width; startX++)
				{
					for (int startY = y; startY < y + pItemInfo->height; startY++)
					{

						if (startY >= capacityHeight)
						{
							spaceYCount = 0;
							break;
						}

						//해당 인벤에 만약에 빈칸이 하나라도 통째로 무쓸모.
						if (invenVisualize[startX][startY] == false)
						{
							spaceYCount++;
						}
						else
						{
							spaceXCount = 0;
							break;
						}
					}
					if (spaceYCount == 0)
					{
						spaceXCount = 0;
						break;
					}
					else
					{
						spaceXCount++;
					}
				}

				//해당 공간이 사용할 수 있는 공간이면 그자리에서 끝내고 아니면 계속 탐색
				if (spaceXCount == pItemInfo->width && spaceYCount == pItemInfo->height)
				{
					//TODO:tuple로  포지션도 같이 리턴할것.
					return  std::tuple<bool, int, int>(true, x, y);
				}
				else
				{
					//이미 height 만큼 해당 지점을 검색했기 때문에 다음 지점으로 패스 시킨다.
					yIncrease += pItemInfo->height;
				}
			}
		}
	}
	//이미 윗단계에서 체크를 못했다면 없다는 것이므로 빠져나감.
	return std::tuple<bool, int, int>(false, -1, -1);
}

void UBackpack::UpdateInvenVisualize(UItemInfo* pItemInfo)
{
	for (int y = pItemInfo->top; y < pItemInfo->top + pItemInfo->height; y++)
	{
		for (int x = pItemInfo->left; x < pItemInfo->left + pItemInfo->width; x++)
		{
			invenVisualize[x][y] = true;
		}
	}
}

void UBackpack::RemoveInvenVisualize(UItemInfo* pItemInfo)
{
	for (int y = pItemInfo->top; y < pItemInfo->top + pItemInfo->height; y++)
	{
		for (int x = pItemInfo->left; x < pItemInfo->left + pItemInfo->width; x++)
		{
			invenVisualize[x][y] = false;
		}
	}

}

void UBackpack::CleanupBackpack()
{
	bool isChanged = false;
	for (int i = 0 ; i < itemContainers.Num() ; i++)
	{
		if (itemContainers[i]->currentCapacity <= 0)
		{
			RemoveInvenVisualize(itemContainers[i]);
			itemContainers.RemoveAt(i);
			isChanged = true;
			break;
		}
	}
	if (isChanged)
	{
		CleanupBackpack();
	}
}

std::tuple<bool, int, int> UBackpack::HasEmptySpace(UItemInfo* pItemInfo)
{
	if (pItemInfo->width >= pItemInfo->height)
	{
			return HasEmptySpaceWidthAxis(pItemInfo);
	}
	else
	{
			return HasEmptySpaceHeightAxis(pItemInfo);
	}


	return std::tuple<bool, int, int>(false, -1, -1);
}


bool UBackpack::IsIntersected(UItemInfo* pItemInfo)
{
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		if (FSlateRect::DoRectanglesIntersect(itemContainers[i]->rect, pItemInfo->rect))
			return true;
	}
	return false;
}

bool UBackpack::UseItem(UItemInfo* pItemInfo)
{
	UItemInfo* itemRef = GetItemReference(pItemInfo);
	itemRef->currentCapacity -= pItemInfo->consumeAmount;
	if (itemRef->currentCapacity <= 0)
		return true;
	else
		return false;
}

bool UBackpack::AddItem(UItemInfo* pItemInfo,UInventory* pInventory)
{
	//TODO: 아이템 빈자리 찾아서 추가 
	std::tuple<bool, int, int> results = HasEmptySpace(pItemInfo); //자리 여부 , 해당 아이템의 left,top
	if (std::get<0>(results))
	{
		pItemInfo->InitRect(std::get<1>(results), std::get<2>(results));
		UpdateInvenVisualize(pItemInfo);
		pItemInfo->refInventory = pInventory;
		itemContainers.Add(pItemInfo);
		return true;
	}
	else
	{
		return false;
	}
}

bool UBackpack::HasItem(UItemInfo* pItemInfo)
{
	bool isItemContains = (GetItemReference(pItemInfo) != nullptr);
	if (isItemContains)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UBackpack::ActualRemoveItem(UItemInfo* pItemInfo)
{
	RemoveInvenVisualize(pItemInfo);
	itemContainers.Remove(pItemInfo);
}

void UBackpack::UpdateAndCleanupBackpack()
{
	CleanupBackpack();
}

int UBackpack::GetAllPrimaryWeaponAmmo(FString pWeaponName)
{
	int sum = 0;
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item type : %d, item weapon class : %s , item weapon param class : %s"), itemContainers[i]->itemType,
			*itemContainers[i]->weaponSubclass->GetName(),
			*pWeaponName);
		if (itemContainers[i]->itemType == ItemType::MAGAZINE && itemContainers[i]->weaponSubclass->GetName().Equals(pWeaponName))
		{
			sum += itemContainers[i]->currentCapacity;
		}
	}

	return sum;
}

int UBackpack::GetAllSecondaryWeaponAmmo(FString pWeaponName)
{
	int sum = 0;
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item type : %d, item weapon class : %s , item weapon param class : %s"), itemContainers[i]->itemType,
			*itemContainers[i]->weaponSubclass->GetName(),
			*pWeaponName);
		if (itemContainers[i]->itemType == ItemType::MAGAZINE && itemContainers[i]->weaponSubclass->GetName().Equals(pWeaponName))
		{
			sum += itemContainers[i]->currentCapacity;
		}
	}

	return sum;
}

//TODO: 만약에 이게 성능에 문제가 될 경우, 스타트 인덱스를 파라미터로 넘겨줄것.

void UBackpack::UsePrimaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName)
{
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		if (itemContainers[i]->itemType == ItemType::MAGAZINE && 
			itemContainers[i]->weaponSubclass->GetName().Equals(pWeaponClassName) && 
			itemContainers[i]->currentCapacity > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item type : %d, item weapon class : %s , item weapon param class : %s , amount : %d"), itemContainers[i]->itemType,
				*itemContainers[i]->weaponSubclass->GetName(),
				*pWeaponClassName,
				itemContainers[i]->currentCapacity);
			if (itemContainers[i]->currentCapacity >= pUseAmmo)
			{
				itemContainers[i]->currentCapacity -= pUseAmmo;
				break;
			}
			else
			{
				int left = pUseAmmo - itemContainers[i]->currentCapacity;
				itemContainers[i]->currentCapacity = 0;
				UsePrimaryWeaponAmmo(left, pWeaponClassName);
				break;
			}
		}
	}
}

//TODO: 만약에 이게 성능에 문제가 될 경우, 스타트 인덱스를 파라미터로 넘겨줄것.

void UBackpack::UseSecondaryWeaponAmmo(int pUseAmmo, FString pWeaponClassName)
{
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		if (itemContainers[i]->itemType == ItemType::MAGAZINE 
			&& itemContainers[i]->weaponSubclass->GetName().Equals(pWeaponClassName) 
			&& itemContainers[i]->currentCapacity > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item type : %d, item weapon class : %s , item weapon param class : %s , amount : %d"), itemContainers[i]->itemType,
				*itemContainers[i]->weaponSubclass->GetName(),
				*pWeaponClassName,
				itemContainers[i]->currentCapacity);
			if (itemContainers[i]->currentCapacity >= pUseAmmo)
			{
				itemContainers[i]->currentCapacity -= pUseAmmo;
				break;
			}
			else
			{
				int left = pUseAmmo - itemContainers[i]->currentCapacity;
				itemContainers[i]->currentCapacity = 0;
				UseSecondaryWeaponAmmo(left, pWeaponClassName);
				break;
			}
		}
	}
}

UItemInfo* UBackpack::GetItemReference(UItemInfo* pItemPtr)
{
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		if (itemContainers[i] == pItemPtr)
		{
			return itemContainers[i];
		}
	}
	return nullptr;
}

FVector2D UBackpack::GetBackpackSize()
{
	return FVector2D(capacityWidth,capacityHeight);
}
