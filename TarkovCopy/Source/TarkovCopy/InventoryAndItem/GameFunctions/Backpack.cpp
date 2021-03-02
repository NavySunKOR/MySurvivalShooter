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

	//���ѿ��� begin destroy�� �����ϰ� �õ��غ�������, UObject isValid ���� assertion ������ ���� ���� �ϰ� ������ ���� �ۼ��Ͽ���.

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
			//�ѹ� width ��ŭ �ǳ� �پ����� �ٽ� ���� ��Ų��.
			xIncrease = 1;

			//�ش�ĭ�� ����ٸ�
				if (invenVisualize[x][y] == false)
				{
					//�� ��ó�� �������� width, height ��ŭ Ž��
					for (int startY = y; startY < y + pItemInfo->height; startY++)
					{
						for (int startX = x; startX < x + pItemInfo->width; startX++)
						{
							//startX�� �κ� ũ�⸦ ����� ���� ���� ó��
							if (startX >= capacityWidth)
							{
								spaceXCount = 0;
								break;
							}

							//�ش� �κ��� ���࿡ ��ĭ�� �ϳ��� ��°�� ������.
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

					//�ش� ������ ����� �� �ִ� �����̸� ���ڸ����� ������ �ƴϸ� ��� Ž��
					if (spaceXCount >= pItemInfo->width && spaceYCount >= pItemInfo->height)
					{
						//TODO:tuple��  �����ǵ� ���� �����Ұ�.
						return  std::tuple<bool, int, int>(true, x, y);
					}
					else
					{
						//�̹� width ��ŭ �ش� ������ �˻��߱� ������ ���� �������� �н� ��Ų��.
						xIncrease += pItemInfo->width;
					}
				}
		}
	}
	//�̹� ���ܰ迡�� üũ�� ���ߴٸ� ���ٴ� ���̹Ƿ� ��������.
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
			//�ѹ� height ��ŭ �ǳ� �پ����� �ٽ� ���� ��Ų��.
			yIncrease = 1;

			//�ش�ĭ�� ����ٸ�
			if (invenVisualize[x][y] == false)
			{
				//�� ��ó�� �������� width, height ��ŭ Ž��
				for (int startX = x; startX < x + pItemInfo->width; startX++)
				{
					for (int startY = y; startY < y + pItemInfo->height; startY++)
					{

						if (startY >= capacityHeight)
						{
							spaceYCount = 0;
							break;
						}

						//�ش� �κ��� ���࿡ ��ĭ�� �ϳ��� ��°�� ������.
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

				//�ش� ������ ����� �� �ִ� �����̸� ���ڸ����� ������ �ƴϸ� ��� Ž��
				if (spaceXCount == pItemInfo->width && spaceYCount == pItemInfo->height)
				{
					//TODO:tuple��  �����ǵ� ���� �����Ұ�.
					return  std::tuple<bool, int, int>(true, x, y);
				}
				else
				{
					//�̹� height ��ŭ �ش� ������ �˻��߱� ������ ���� �������� �н� ��Ų��.
					yIncrease += pItemInfo->height;
				}
			}
		}
	}
	//�̹� ���ܰ迡�� üũ�� ���ߴٸ� ���ٴ� ���̹Ƿ� ��������.
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
	//TODO: ������ ���ڸ� ã�Ƽ� �߰� 
	std::tuple<bool, int, int> results = HasEmptySpace(pItemInfo); //�ڸ� ���� , �ش� �������� left,top
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

//TODO: ���࿡ �̰� ���ɿ� ������ �� ���, ��ŸƮ �ε����� �Ķ���ͷ� �Ѱ��ٰ�.

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

//TODO: ���࿡ �̰� ���ɿ� ������ �� ���, ��ŸƮ �ε����� �Ķ���ͷ� �Ѱ��ٰ�.

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
