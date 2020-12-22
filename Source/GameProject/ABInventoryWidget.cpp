// Fill out your copyright notice in the Description page of Project Settings.


#include "ABInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryComponent.h"
#include "InventorySlot.h"
#include "CharacterStatWidget.h"
#include "Components/TextBlock.h"
#include "CharacterStatComponent.h"
#include "EquipmentSlot.h"
#include "Components/Button.h"
#include "ABPlayerController.h"
#include "PlayerCharacter.h"

void UABInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Inventory = Cast<UUniformGridPanel>(GetWidgetFromName(FName("InventoryGridPanel")));
	
	HeadSlot = Cast<UEquipmentSlot>(GetWidgetFromName(FName("Head")));
	ChestSlot = Cast<UEquipmentSlot>(GetWidgetFromName(FName("Chest")));
	LegsSlot = Cast<UEquipmentSlot>(GetWidgetFromName(FName("Legs")));
	WeaponSlot = Cast<UEquipmentSlot>(GetWidgetFromName(FName("Weapon")));
	ShieldSlot = Cast<UEquipmentSlot>(GetWidgetFromName(FName("Shield")));


	BackBt = Cast<UButton>(GetWidgetFromName(FName("BackButton")));
	BackBt->OnClicked.AddDynamic(this, &UABInventoryWidget::ClickedBackBt);

	Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->SetInventoryWidget(this);
}

bool UABInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return true;
}

UUniformGridPanel* UABInventoryWidget::GetInventory()
{
	return Inventory;
}

void UABInventoryWidget::ChangeHeadSlot(FName ItemName)
{
	HeadSlot->SetItem(ItemName);
}

void UABInventoryWidget::ChangeChestSlot(FName ItemName)
{
	ChestSlot->SetItem(ItemName);
}

void UABInventoryWidget::ChangeLegsSlot(FName ItemName)
{
	LegsSlot->SetItem(ItemName);
}

void UABInventoryWidget::ChangeWeaponSlot(FName ItemName)
{
	WeaponSlot->SetItem(ItemName);
}

void UABInventoryWidget::ChangeShieldSlot(FName ItemName)
{
	ShieldSlot->SetItem(ItemName);
}

void UABInventoryWidget::ClickedBackBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenInventoryWidget();
	Cast<AABPlayerController>(GetOwningPlayer())->VisiblePauseMenu();
}