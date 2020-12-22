// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UPotionSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void BindInventoryComp(class UInventoryComponent* NewInventoryComp);

private:
	class UTextBlock* PotionAmount;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

};
