// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "ABGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "ABInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UABInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	class UUniformGridPanel* GetInventory();

	void ChangeHeadSlot(FName ItemName);
	void ChangeChestSlot(FName ItemName);
	void ChangeLegsSlot(FName ItemName);
	void ChangeWeaponSlot(FName ItemName);
	void ChangeShieldSlot(FName ItemName);

	UFUNCTION()
	void ClickedBackBt();

private:
	/**
	* 인벤토리창 슬롯
	*/
	UPROPERTY()
	class UUniformGridPanel* Inventory;

	/**
	* 장비창 슬롯
	*/
	UPROPERTY()
	class UEquipmentSlot* HeadSlot;

	UPROPERTY()
	class UEquipmentSlot* ChestSlot;

	UPROPERTY()
	class UEquipmentSlot* LegsSlot;

	UPROPERTY()
	class UEquipmentSlot* WeaponSlot;

	UPROPERTY()
	class UEquipmentSlot* ShieldSlot;

	/** 돌아가기 버튼 */
	UPROPERTY()
	class UButton* BackBt;
	//
};
