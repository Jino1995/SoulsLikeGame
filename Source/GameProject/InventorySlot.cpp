// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "DragWidget.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "ItemTooltipWidget.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	AmountText = Cast<UTextBlock>(GetWidgetFromName(FName("ItemAmount")));
	AmountText->SetVisibility(ESlateVisibility::Hidden);

	ItemIcon = Cast<UImage>(GetWidgetFromName(FName("IconImage")));

	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ItemData = ABGameInstance->GetItemData("Default");

}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag))
	{
		int32 ChangeSlot1 = Cast<UUniformGridPanel>(GetParent())->GetChildIndex(this);
		int32 ChangeSlot2 = Cast<UUniformGridPanel>(GetParent())->GetChildIndex(Cast<UDragWidget>(InOperation)->WidgetToDrag);
		
		Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->ChangeItemPosition(ChangeSlot1, ChangeSlot2);

		FName PreItemName = ItemData->GetName();
		int32 PreAmount = ItemAmount;

		SetItem(Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag)->GetItemName(), Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag)->GetItemAmount());
		Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag)->SetItem(PreItemName, PreAmount);
	}
	
	return true;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	UDragWidget* oper = NewObject<UDragWidget>();
	oper->DefaultDragVisual = this;
	oper->WidgetToDrag = this;
	oper->DropWindowOffset = DragWindowOffset;
	oper->Pivot = EDragPivot::CenterCenter;
	OutOperation = oper;
	
}


FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true && ItemData->GetName() != TEXT("Default"))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		if (ItemData->GetItemType() == EItemType::Equipment && Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetbCombatMode() == ECombatMode::NonCombat)
		{
			if (Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->EquipItem(ItemData->GetName(), Cast<UUniformGridPanel>(GetParent())->GetChildIndex(this)))
			{
				SetItem(TEXT("Default"), 0);
			}
		}
	}

	return reply.NativeReply;	
}

void UInventorySlot::SetItem(FName ItemName, int32 Amount)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ItemData = ABGameInstance->GetItemData(ItemName);
	
	ItemIcon->SetBrushFromTexture(ItemData->GetIcon());

	ItemAmount = Amount;
	AmountText->SetText(FText::FromString(FString::FromInt(Amount)));
	if (ItemName == TEXT("Default"))
	{
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}

	ItemInfo = CreateWidget<UItemTooltipWidget>(GetOwningPlayer(), ToolTipWidgetAsset);
	if (ItemData->GetItemType() != EItemType::None)
	{
		Cast<UCanvasPanel>(GetWidgetFromName("Canvas"))->AddChild(ItemInfo);
		ItemInfo->SetItemToolTip(*ItemData);
		ItemInfo->RemoveFromParent();

		SetToolTip(ItemInfo);
	}
	else
	{
		SetToolTip(nullptr);
	}
}