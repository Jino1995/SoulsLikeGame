// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoneyWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UMoneyWidget : public UUserWidget
{
	GENERATED_BODY()
		
protected:
	virtual void NativeConstruct() override;

private:
	class UTextBlock* Money;

	class UInventoryComponent* CharacterInventory;

};
