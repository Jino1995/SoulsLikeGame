// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ABGameInstance.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlot.h"
#include "SystemTextContainerWidget.h"
#include "PlayerCharacter.h"
#include "CharacterStatComponent.h"
#include "ABSaveGame.h"
#include "ABPlayerController.h"
#include "HUDOverlay.h"
#include "ABInventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ClearInventory();

	Equipments.Emplace(EEquipmentType::Head , TEXT("Default"));
	Equipments.Emplace(EEquipmentType::Chest, TEXT("Default"));
	Equipments.Emplace(EEquipmentType::Legs, TEXT("Default"));
	Equipments.Emplace(EEquipmentType::Weapon, TEXT("Default"));
	Equipments.Emplace(EEquipmentType::Shield, TEXT("Default"));

	MaxPotionNum = 5;
	CurrentPotionNum = MaxPotionNum;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterStat = Cast<APlayerCharacter>(GetOwner())->GetCharacterStatComponent();

	CharacterStat->OnCONStatChanged.AddLambda([this]()-> void {
		MaxWeight = 100;
		IncMaxWeight(CharacterStat->GetMaxWeightPerOneCON() * CharacterStat->GetCONStat());
	});
	
	MaxWeight = 100;
	CurrentWeight = 0;
	OnMaxWeightChanged.Broadcast();
	OnCurrentWeightChanged.Broadcast();

	// ...
	
}

TArray<FInventorySlotData> UInventoryComponent::GetInventory()
{
	return Items;
}

bool UInventoryComponent::IsEmpty()
{
	if (Items.Num())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UInventoryComponent::ClearInventory()
{
	Items.Init(FInventorySlotData(TEXT("Default"), 0), MAX_INVENTORY_SIZE);
}

void UInventoryComponent::ChangeItemPosition(int32 Pos1, int32 Pos2)
{
	FInventorySlotData BackUpData = Items[Pos1];
	Items[Pos1] = Items[Pos2];
	Items[Pos2] = BackUpData;
}

bool UInventoryComponent::AddItem(FName NewItem, int32 ItemNum, bool bSystemText)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(NewItem);
	int i = 0;
	
	if (ItemData->GetIsStackable())
	{
		for (auto& Item : Items)
		{
			if (Item.ItemName == NewItem && Item.Amount + ItemNum <= ItemData->GetMaxStackSize())
			{
				Item.Amount += ItemNum;
				Cast<UInventorySlot>(InventoryWidget->GetInventory()->GetChildAt(i))->SetItem(Item.ItemName, Item.Amount);
				if (bSystemText)
				{
					SystemTextContainer->CreateAddItemText(Item.ItemName);
				}
				return true;
			}
			else if (Item.ItemName == NewItem && Item.Amount + ItemNum > ItemData->GetMaxStackSize())
			{
				Item.Amount = ItemData->GetMaxStackSize();
				Cast<UInventorySlot>(InventoryWidget->GetInventory()->GetChildAt(i))->SetItem(Item.ItemName, Item.Amount);
				if (bSystemText)
				{
					SystemTextContainer->CreateAddItemText(Item.ItemName);
				}
			}
			else if (Item.ItemName == TEXT("Default"))
			{
				Item.ItemName = NewItem;
				Item.Amount = 1;
				Cast<UInventorySlot>(InventoryWidget->GetInventory()->GetChildAt(i))->SetItem(Item.ItemName, Item.Amount);
				if (bSystemText)
				{
					SystemTextContainer->CreateAddItemText(Item.ItemName);
				}
				return true;
			}
			i++;
		}
		SystemTextContainer->CreateInventoryFullText();
		return false;
	}
	else
	{
		for (auto& Item : Items)
		{
			if (Item.ItemName == TEXT("Default"))
			{
				Item.ItemName = NewItem;
				Item.Amount = 1;
				Cast<UInventorySlot>(InventoryWidget->GetInventory()->GetChildAt(i))->SetItem(Item.ItemName, Item.Amount);
				if (bSystemText)
				{
					SystemTextContainer->CreateAddItemText(Item.ItemName);
				}
				return true;
			}
			i++;
		}
		SystemTextContainer->CreateInventoryFullText();
		return false;
	}
}

void UInventoryComponent::DropItem(int32 SlotNum)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(Items[SlotNum].ItemName);
	DecCurrentWeight(ItemData->GetWeight() * Items[SlotNum].Amount);
	Items[SlotNum] = FInventorySlotData(TEXT("Default"), 0);
}

void UInventoryComponent::SetInventoryWidget(UABInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}


void UInventoryComponent::SetSystemTextContainer(class USystemTextContainerWidget* NewSystemTextContainer)
{
	SystemTextContainer = NewSystemTextContainer;
}


void UInventoryComponent::IncMaxWeight(int32 AddedWeight)
{
	MaxWeight += AddedWeight;
	OnMaxWeightChanged.Broadcast();
}

bool UInventoryComponent::IncCurrentWeight(int32 AddedWeight)
{
	if (CurrentWeight + AddedWeight > MaxWeight)
	{
		return false;
	}
	else
	{
		CurrentWeight += AddedWeight;
		OnCurrentWeightChanged.Broadcast();
		return true;
	}
}

void UInventoryComponent::DecMaxWeight(int32 DecWeight)
{
	MaxWeight -= DecWeight;
	OnMaxWeightChanged.Broadcast();
}

void UInventoryComponent::DecCurrentWeight(int32 DecWeight)
{
	CurrentWeight -= DecWeight;
	OnCurrentWeightChanged.Broadcast();
}

void UInventoryComponent::EquipItemImmediately(FName ItemName)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(ItemName);

	switch (ItemData->GetEquipmentType())
	{
	case EEquipmentType::Head:
		Equipments[EEquipmentType::Head] = ItemName;
		Cast<APlayerCharacter>(GetOwner())->GetCharacterStat()->IncStatForItem(Equipments[EEquipmentType::Head]);
		Cast<AABPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController())->GetInventoryWidget()->ChangeHeadSlot(ItemName);
		break;
	case EEquipmentType::Chest:
		Equipments[EEquipmentType::Chest] = ItemName;
		Cast<APlayerCharacter>(GetOwner())->GetCharacterStat()->IncStatForItem(Equipments[EEquipmentType::Chest]);
		Cast<AABPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController())->GetInventoryWidget()->ChangeChestSlot(ItemName);
		break;
	case EEquipmentType::Legs:
		Equipments[EEquipmentType::Legs] = ItemName;
		Cast<APlayerCharacter>(GetOwner())->GetCharacterStat()->IncStatForItem(Equipments[EEquipmentType::Legs]);
		Cast<AABPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController())->GetInventoryWidget()->ChangeLegsSlot(ItemName);
		break;
	case EEquipmentType::Weapon:
		Equipments[EEquipmentType::Weapon] = ItemName;
		Cast<APlayerCharacter>(GetOwner())->GetCharacterStat()->IncStatForItem(Equipments[EEquipmentType::Weapon]);
		Cast<APlayerCharacter>(GetOwner())->EquipWeapon(ItemData->GetWeaponBP());
		Cast<AABPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController())->GetInventoryWidget()->ChangeWeaponSlot(ItemName);
		break;
	case EEquipmentType::Shield:
		Equipments[EEquipmentType::Shield] = ItemName;
		Cast<APlayerCharacter>(GetOwner())->GetCharacterStat()->IncStatForItem(*(Equipments.Find(EEquipmentType::Shield)));
		Cast<APlayerCharacter>(GetOwner())->EquipShield(ItemData->GetShieldBP());
		Cast<AABPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController())->GetInventoryWidget()->ChangeShieldSlot(ItemName);
		break;
	}
}

bool UInventoryComponent::EquipItem(FName ItemName, int32 SlotNum)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(ItemName);

	switch (ItemData->GetEquipmentType())
	{
	case EEquipmentType::Head:
		if (Equipments[EEquipmentType::Head] == TEXT("Default"))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Head] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Head]);
			InventoryWidget->ChangeHeadSlot(ItemName);
			return true;
		}
		else if (UnEquipItem(EEquipmentType::Head))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Head] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Head]);
			InventoryWidget->ChangeHeadSlot(ItemName);
			return true;
		}
		break;
	case EEquipmentType::Chest:
		if (Equipments[EEquipmentType::Chest] == TEXT("Default"))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Chest] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Chest]);
			InventoryWidget->ChangeChestSlot(ItemName);
			return true;
		}
		else if (UnEquipItem(EEquipmentType::Chest))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Chest] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Chest]);
			InventoryWidget->ChangeChestSlot(ItemName);
			return true;
		}
		break;
	case EEquipmentType::Legs:
		if (Equipments[EEquipmentType::Legs] == TEXT("Default"))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Legs] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Legs]);
			InventoryWidget->ChangeLegsSlot(ItemName);
			return true;
		}
		else if (UnEquipItem(EEquipmentType::Legs))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Legs] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Legs]);
			InventoryWidget->ChangeLegsSlot(ItemName);
			return true;
		}
		break;
	case EEquipmentType::Weapon:
		if (ItemData->GetWeaponType() == EWeaponType::TwoHanded)
		{
			int32 NeedSlotNum = 0;
			if (Equipments[EEquipmentType::Weapon] == TEXT("Default")) NeedSlotNum++;
			if (Equipments[EEquipmentType::Shield] == TEXT("Default")) NeedSlotNum++;

			if (EmptyInventorySlotNum() < NeedSlotNum)
			{
				SystemTextContainer->CreateInventoryFullText();
				return false;
			}
			else
			{
				UnEquipItem(EEquipmentType::Weapon);
				UnEquipItem(EEquipmentType::Shield);

				Items[SlotNum].ItemName = TEXT("Default");
				Equipments[EEquipmentType::Weapon] = ItemName;
				CharacterStat->IncStatForItem(Equipments[EEquipmentType::Weapon]);
				Cast<APlayerCharacter>(GetOwner())->EquipWeapon(ItemData->GetWeaponBP());
				InventoryWidget->ChangeWeaponSlot(ItemName);
				return true;
			}
		}
		else
		{
			if (Equipments[EEquipmentType::Weapon] == TEXT("Default"))
			{
				Items[SlotNum].ItemName = TEXT("Default");
				Equipments[EEquipmentType::Weapon] = ItemName;
				CharacterStat->IncStatForItem(Equipments[EEquipmentType::Weapon]);
				Cast<APlayerCharacter>(GetOwner())->EquipWeapon(ItemData->GetWeaponBP());
				InventoryWidget->ChangeWeaponSlot(ItemName);
				return true;
			}
			else if (UnEquipItem(EEquipmentType::Weapon))
			{
				Items[SlotNum].ItemName = TEXT("Default");
				Equipments[EEquipmentType::Weapon] = ItemName;
				CharacterStat->IncStatForItem(Equipments[EEquipmentType::Weapon]);
				Cast<APlayerCharacter>(GetOwner())->EquipWeapon(ItemData->GetWeaponBP());
				InventoryWidget->ChangeWeaponSlot(ItemName);
				return true;
			}
		}
		break;
	case EEquipmentType::Shield:
		FItemData* WeaponData = ABGameInstance->GetItemData(Equipments[EEquipmentType::Weapon]);
		if (WeaponData->GetWeaponType() == EWeaponType::TwoHanded)
		{
			UnEquipItem(EEquipmentType::Weapon);
			Cast<APlayerCharacter>(GetOwner())->UnEquipWeapon();
		}

		if (Equipments[EEquipmentType::Shield] == TEXT("Default"))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Shield] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Shield]);
			Cast<APlayerCharacter>(GetOwner())->EquipShield(ItemData->GetShieldBP());
			InventoryWidget->ChangeShieldSlot(ItemName);
			return true;
		}
		else if (UnEquipItem(EEquipmentType::Shield))
		{
			Items[SlotNum].ItemName = TEXT("Default");
			Equipments[EEquipmentType::Shield] = ItemName;
			CharacterStat->IncStatForItem(Equipments[EEquipmentType::Shield]);
			Cast<APlayerCharacter>(GetOwner())->EquipShield(ItemData->GetShieldBP());
			InventoryWidget->ChangeShieldSlot(ItemName);
			return true;
		}
		break;
	}
	return false;
}

bool UInventoryComponent::UnEquipItem(EEquipmentType ItemType)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(Equipments[ItemType]);

	if (Equipments[ItemType] == TEXT("Default"))
	{
		return false;
	}
	else if (AddItem(Equipments[ItemType]))
	{
		CharacterStat->DecStatForItem(Equipments[ItemType]);
		Equipments[ItemType] = TEXT("Default");
		switch (ItemType)
		{
		case EEquipmentType::Weapon:
			Cast<APlayerCharacter>(GetOwner())->UnEquipWeapon();
			InventoryWidget->ChangeWeaponSlot(TEXT("Default"));
			break;
		case EEquipmentType::Shield:
			Cast<APlayerCharacter>(GetOwner())->UnEquipShield();
			InventoryWidget->ChangeShieldSlot(TEXT("Default"));
			break;
		default:
			break;
		}
		return true;
	}
	IncCurrentWeight(ItemData->GetWeight());
	return false;
}

void UInventoryComponent::IncMoney(int32 IncAmount)
{
	Money += IncAmount;
	OnMoneyChanged.Broadcast();
}

bool UInventoryComponent::DecMoney(int32 DecAmount)
{
	if ((Money - DecAmount) < 0)
	{
		return false;
	}
	else
	{
		Money -= DecAmount;
		OnMoneyChanged.Broadcast();
		return true;
	}
}

void UInventoryComponent::FillUpPotion()
{
	CurrentPotionNum = MaxPotionNum;
	OnPotionNumChanged.Broadcast();
}

bool UInventoryComponent::UsePotion()
{
	if (CurrentPotionNum > 0)
	{
		CurrentPotionNum--;
		OnPotionNumChanged.Broadcast();
		return true;
	}
	else
	{
		return false;
	}
}

int32 UInventoryComponent::EmptyInventorySlotNum()
{
	int32 EmptySlotNum = 0;

	for (auto Item : Items)
	{
		if (Item.ItemName == TEXT("Default"))
		{
			EmptySlotNum++;
		}
	}
	return EmptySlotNum;
}

void UInventoryComponent::SaveCharacterItemInfo(UABSaveGame* SaveInstance)
{
	SaveInstance->Money = Money;
	SaveInstance->Items = Items;
	SaveInstance->Equipments = Equipments;

	OnMoneyChanged.Broadcast();
}

void UInventoryComponent::LoadCharacterItemInfo(UABSaveGame* LoadInstance)
{
	Money = LoadInstance->Money;
	for (auto& Item : LoadInstance->Items)
	{
		if (Item.ItemName == FName("Default")) continue;
		AddItem(Item.ItemName, Item.Amount, false);
	}

	for (auto& Equipment : LoadInstance->Equipments)
	{
		if (Equipment.Value == FName("Default")) continue;
		EquipItemImmediately(Equipment.Value);
	}

	OnMoneyChanged.Broadcast();
}