// Fill out your copyright notice in the Description page of Project Settings.

#include "Backpack.h"
#include "TarkovCopy/InventoryAndItem/ItemInfos/ItemInfo.h"
#include <Layout/SlateRect.h>


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
	if (itemContainers.Num() > 0)
	{
		for (int i = itemContainers.Num() - 1 ; i >= 0 ; i--)
		{
			itemContainers.RemoveAt(i);
		}
	}
}

bool  UBackpack::HasEmptySpace(FSlateRect pIntSlateRect)
{
	if (pIntSlateRect.Right > capacityWidth || pIntSlateRect.Bottom > capacityHeight)
	{
		return false;
	}

	for (int x = pIntSlateRect.Left; x < pIntSlateRect.Right; x++)
	{
		for (int y = pIntSlateRect.Top; y < pIntSlateRect.Bottom; y++)
		{
			if (invenVisualize[x][y] == true)
			{
				return false;
			}
		}
	}

	return true;
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
void CleanupItemContainers(TArray<UItemInfo*>& arr)
{
	for (int i = arr.Num() - 1; i >= 0; i--)
	{
		if (arr[i] == nullptr)
		{
			arr.RemoveAt(i);
		}
	}
}

void UBackpack::CleanupBackpack()
{
	bool isChanged = false;
	TArray<UItemInfo*> copyItemContainers = itemContainers;
	for (int i = copyItemContainers.Num() - 1; i >= 0; i--)
	{
		if (copyItemContainers[i]->currentCapacity <= 0)
		{
			RemoveInvenVisualize(copyItemContainers[i]);
			copyItemContainers[i] = nullptr;
		}
	} 
	CleanupItemContainers(copyItemContainers);

	itemContainers = copyItemContainers;
}

std::tuple<bool, int, int> UBackpack::HasEmptySpace(UItemInfo* pItemInfo)
{
	return HasEmptySpaceWidthAxis(pItemInfo); //TODO: ���߿� �������� ���η� �������� �߰� �ϰ� �Ұ��� �����ٰ�
}


bool UBackpack::IsIntersected(UItemInfo* pItemInfo) const
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
	if (itemRef != nullptr)
	{
		itemRef->currentCapacity -= pItemInfo->consumeAmount;
		if (itemRef->currentCapacity <= 0)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

//TODO:�ű� ������ ������� �̸� �ٲܰ�
bool UBackpack::AddNewItem(UItemInfo* pItemInfo,UInventory* pInventory)
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

void UBackpack::AddItemContainerArray(UItemInfo* pItemInfo)
{
	itemContainers.Add(pItemInfo);
}


void UBackpack::DeleteItem(UItemInfo* pItemInfo)
{
	RemoveInvenVisualize(pItemInfo);
	itemContainers.Remove(pItemInfo);
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

UItemInfo* UBackpack::HasItemType(ItemType pItemType)
{
	bool isItemContains = false;
	for (int i = 0; i < itemContainers.Num(); i++)
	{
		if (itemContainers[i]->itemType == pItemType)
		{
			return itemContainers[i];
		}
	}

	return nullptr;
}

void UBackpack::UpdateAndCleanupBackpack()
{
	CleanupBackpack();
}

void UBackpack::MoveItemPosition(UItemInfo* pItemInfo)
{
	UpdateInvenVisualize(pItemInfo);
}

void UBackpack::RemoveItemPosition(UItemInfo* pItemInfo)
{
	RemoveInvenVisualize(pItemInfo);
}

int UBackpack::GetAllPrimaryWeaponAmmo(FString pWeaponName)
{
	int sum = 0;
	for (UItemInfo* item : itemContainers)
	{
		if (item->itemType == ItemType::MAGAZINE && item->weaponSubclass->GetName().Equals(pWeaponName))
		{
			sum += item->currentCapacity;
		}
	}

	return sum;
}

int UBackpack::GetAllSecondaryWeaponAmmo(FString pWeaponName)
{
	int sum = 0;
	for (UItemInfo* item : itemContainers)
	{
		if (item->itemType == ItemType::MAGAZINE && item->weaponSubclass->GetName().Equals(pWeaponName))
		{
			sum += item->currentCapacity;
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
		//��� �Ѱ��� �ִٸ�
		if (itemContainers[i] == pItemPtr && itemContainers[i]->currentCapacity >0)
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
