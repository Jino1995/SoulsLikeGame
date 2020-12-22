// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABGameInstance.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	void SetItem(FName ItemName, int32 Amount);

	FORCEINLINE FName GetItemName() { return ItemData->GetName(); }
	FORCEINLINE int32 GetItemAmount() { return ItemAmount; }

private:
	FItemData* ItemData;

	class UTextBlock* AmountText;
	class UImage* ItemIcon;

	int32 ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventorySlot", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UItemTooltipWidget> ToolTipWidgetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventorySlot", Meta = (AllowPrivateAccess = true))
	class UItemTooltipWidget* ItemInfo;

};