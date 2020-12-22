// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UChapterButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetParentWidget(class UPortalWidget* NewParentWidget);

	UFUNCTION()
	void ClickedChapterButton();

	void ResetWidgetState();

	FORCEINLINE class UButton* GetSlotButton() { return SlotBt; }
	FORCEINLINE FName GetPortLevelName() { return PortLevelName; }
	FORCEINLINE TSubclassOf<class UPortalDetailWidget> GetPortDetailWidgetAsset() { return DetailWidgetAsset; }

private:
	UPROPERTY()
	UPortalWidget* ParentWidget;

	UPROPERTY()
	class UButton* SlotBt;

	UPROPERTY()
	class UTextBlock* ChapterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPortalDetailWidget> DetailWidgetAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FString ChapterNameTxt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FName PortLevelName;
};
