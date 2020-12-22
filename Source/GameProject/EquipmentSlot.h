// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABGameInstance.h"
#include "EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetItem(FName ItemName);
	void SetIcon(UTexture2D* NewIcon);

	FORCEINLINE FName GetItemName() { return ItemData->GetName(); }
	FORCEINLINE EWeaponType GetWeaponType() { return ItemData->GetWeaponType(); }

private:
	FItemData* ItemData;
	class UInventoryWindow* InvWindow;
	class UImage* ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventorySlot", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UItemTooltipWidget> ToolTipWidgetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventorySlot", Meta = (AllowPrivateAccess = true))
	class UItemTooltipWidget* ItemInfo;
};

