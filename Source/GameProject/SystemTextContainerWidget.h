// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SystemTextContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API USystemTextContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void CreateAddItemText(FName NewItemName);
	void CreateInventoryFullText();
	void CreateWeightFullText();

private:
	UPROPERTY()
	class UCanvasPanel* Canvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> AddItemText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> InventoryFullText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> WeightFullText;

};
