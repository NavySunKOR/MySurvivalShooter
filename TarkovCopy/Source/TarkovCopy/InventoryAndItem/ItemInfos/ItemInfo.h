// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TarkovCopy/Weapons/BaseGun.h"
#include "ItemInfo.generated.h"

class APickableItem;
class UInventory;
class AFPPlayerController;
/**
 * 
 */


UENUM()
enum class ItemType :uint8
{
	UNDEFINED UMETA(DisplayName = "undefined"),
	WEAPON UMETA(DisplayName = "weapon"),
	MAGAZINE UMETA(DisplayName = "magazine"),
	ARMOR UMETA(DisplayName = "armor"),
	MEDIKIT UMETA(DisplayName = "medikit"),
	GRENADE UMETA(DisplayName = "grenade"),
	FLASHGRENADE UMETA(DisplayName = "flashGrenade")
};

UCLASS(Blueprintable, BlueprintType)
class TARKOVCOPY_API UItemInfo : public UObject
{
	GENERATED_BODY()
public:

	//UItemInfo();
	//UItemInfo(float leftPos, float topPos, float width, float height);
	//virtual ~UItemInfo();

	UPROPERTY(EditAnywhere)
	ItemType itemType;

	UPROPERTY(EditAnywhere)
	int width;
	UPROPERTY(EditAnywhere)
	int height;

	int left;
	int top;

	UPROPERTY(EditAnywhere)
	bool isConsumable;

	UPROPERTY(EditAnywhere)
	bool isRightClickUsable;
	
	int consumeAmount = 1;

	UPROPERTY(EditAnywhere)
	int maxCapacity;
	UPROPERTY(EditAnywhere)
	int currentCapacity;
	UPROPERTY(EditAnywhere)
	UTexture2D* spriteToUse;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickableItem> meshToDrop;

	//����ų� źâ�̸� �ش� ����Ÿ���� �������ٰ�(������ ����)
	UPROPERTY(EditAnywhere) 
	TSubclassOf<ABaseGun> weaponSubclass; // TODO: subclass �϶��� ��� �߰��� ����ߵȴ�. ���� �������� �ȵȴ�.


	UInventory* refInventory;

	FSlateRect rect;

	virtual void Use(AFPPlayerController* pPlayerCon); // Return : isEmpty
	virtual void DropItem(AFPPlayerController* pPlayerCon);
	virtual void InitRect(float pLeft, float pTop);
	virtual FString GetItemAmountString() const;

	/*UItemInfo operator=(UItemInfo &other);*/

};
