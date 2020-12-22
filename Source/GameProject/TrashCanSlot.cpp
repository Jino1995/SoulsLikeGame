// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashCanSlot.h"
#include "InventorySlot.h"
#include "DragWidget.h"

bool UTrashCanSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag))
	{
		Cast<UInventorySlot>(Cast<UDragWidget>(InOperation)->WidgetToDrag)->SetItem(FName("Default"), 0);
		return true;
	}

	return false;
}
