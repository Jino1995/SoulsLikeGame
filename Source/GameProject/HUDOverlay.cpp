// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDOverlay.h"
#include "CharacterStateWidget.h"
#include "InventoryComponent.h"
#include "SystemTextContainerWidget.h"
#include "InventorySlot.h"
#include "DragWidget.h"
#include "PlayerCharacter.h"
#include "Components/UniformGridPanel.h"
#include "PotionSlot.h"
#include "BossHPBar.h"
#include "TimerManager.h"

void UHUDOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	StateWidget = Cast<UCharacterStateWidget>(GetWidgetFromName(FName("PlayerState")));

	SystemTextContainerWidget = Cast<USystemTextContainerWidget>(GetWidgetFromName(FName("SystemTextContainer")));
	
	PotionSlotWidget = Cast<UPotionSlot>(GetWidgetFromName("PotionSlot"));

	BossHPBarWidget = Cast<UBossHPBar>(GetWidgetFromName("BossHPBar"));
	BossHPBarWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool UHUDOverlay::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag))
	{
		int32 SlotNum = Cast<UUniformGridPanel>(Cast<UDragWidget>(InOperation)->WidgetToDrag->GetParent())->GetChildIndex(Cast<UDragWidget>(InOperation)->WidgetToDrag);
		Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->DropItem(SlotNum);
		Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag)->SetItem(TEXT("Default"), 0);
	}

	return true;
}

void UHUDOverlay::BindCharacterStat(class UCharacterStatComponent* StatComp)
{
	StateWidget->BindCharacterStat(StatComp);
}

void UHUDOverlay::BindInventoryComp(UInventoryComponent* InvenComp)
{
	InvenComp->SetSystemTextContainer(SystemTextContainerWidget);
}

void UHUDOverlay::VisibleBossHPBarWidget(class AEnemy* TargetBoss)
{
	BossHPBarWidget->BindMonsterStat(TargetBoss);
	BossHPBarWidget->SetVisibility(ESlateVisibility::Visible);
}

void UHUDOverlay::HiddenBossHPBarWidgetDelay()
{
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &UHUDOverlay::HiddenBossHPBarWidget, 10.f, false);
}

void UHUDOverlay::HiddenBossHPBarWidget()
{
	BossHPBarWidget->SetVisibility(ESlateVisibility::Hidden);
}