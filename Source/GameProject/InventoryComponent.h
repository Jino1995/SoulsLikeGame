// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "ABGameInstance.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMaxWeightChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCurrentWeightChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMoneyChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnPotionNumChangedDelegate);

USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()

public:
	FInventorySlotData() : ItemName(FName("Default")), Amount(0) {}
	FInventorySlotData(FName NewItemName, int32 NewAmount) 
	{ 
		ItemName = NewItemName; 
		Amount = NewAmount; 
	}

	UPROPERTY(VisibleAnywhere)
	FName ItemName;

	UPROPERTY(VisibleAnywhere)
	int32 Amount;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	TArray<FInventorySlotData> GetInventory();
	bool IsEmpty();

	void ClearInventory();

	void ChangeItemPosition(int32 Pos1, int32 Pos2);

	bool AddItem(FName NewItem, int32 ItemNum = 1, bool bSystemText = true);
	void DropItem(int32 SlotNum);

	void SetInventoryWidget(class UABInventoryWidget* NewInventoryWidget);

	void SetSystemTextContainer(class USystemTextContainerWidget* NewSystemTextContainer);

	void IncMaxWeight(int32 AddedWeight);
	bool IncCurrentWeight(int32 AddedWeight);
	void DecMaxWeight(int32 DecWeight);
	void DecCurrentWeight(int32 DecWeight);

	void EquipItemImmediately(FName ItemName);
	bool EquipItem(FName ItemName, int32 SlotNum);
	bool UnEquipItem(EEquipmentType ItemType);

	void IncMoney(int32 IncAmount);
	bool DecMoney(int32 DecAmount);

	void FillUpPotion();
	bool UsePotion();

	int32 EmptyInventorySlotNum();

	void SaveCharacterItemInfo(class UABSaveGame* SaveInstance);
	void LoadCharacterItemInfo(class UABSaveGame* LoadInstance);

	FORCEINLINE int32 GetMaxWeight() { return MaxWeight; }
	FORCEINLINE int32 GetCurrentWeight() { return CurrentWeight; }
	FORCEINLINE int32 GetCurrentPotionNum() { return CurrentPotionNum; }
	FORCEINLINE float GetPotionRecovery() { return PotionRecovery; }
	FORCEINLINE int32 GetCurrentMoney() { return Money; }

public:
	FOnMaxWeightChangedDelegate OnMaxWeightChanged;
	FOnCurrentWeightChangedDelegate OnCurrentWeightChanged;
	FOnMoneyChangedDelegate OnMoneyChanged;
	FOnPotionNumChangedDelegate OnPotionNumChanged;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FInventorySlotData> Items;

	UPROPERTY(VisibleAnywhere)
	TMap<EEquipmentType, FName> Equipments;

	UPROPERTY()
	class UABInventoryWidget* InventoryWidget;

	UPROPERTY()
	class USystemTextContainerWidget* SystemTextContainer;

	int32 Money;

	int32 MaxWeight;

	int32 CurrentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Potion, Meta = (AllowPrivateAccess = true))
	int32 MaxPotionNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Potion, Meta = (AllowPrivateAccess = true))
	int32 CurrentPotionNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Potion, Meta = (AllowPrivateAccess = true))
	float PotionRecovery;

	UPROPERTY()
	class UCharacterStatComponent* CharacterStat;

	const int32 MAX_INVENTORY_SIZE = 24;
};
