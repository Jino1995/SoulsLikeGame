// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlot.h"
#include "Components/Image.h"
#include "ItemTooltipWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"

void UEquipmentSlot::NativeConstruct()
{
	ItemIcon = Cast<UImage>(GetWidgetFromName(FName("IconImage")));

	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ItemData = ABGameInstance->GetItemData("Default");
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true && Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetbCombatMode() == ECombatMode::NonCombat)
	{
		if (Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->UnEquipItem(ItemData->GetEquipmentType()))
		{
			SetItem(TEXT("Default"));
		}
	}

	return reply.NativeReply;
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}


void UEquipmentSlot::SetItem(FName ItemName)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ItemData = ABGameInstance->GetItemData(ItemName);

	ItemIcon->SetBrushFromTexture(ItemData->GetIcon());
	
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

void UEquipmentSlot::SetIcon(UTexture2D* NewIcon)
{
	ItemIcon->SetBrushFromTexture(NewIcon);
}