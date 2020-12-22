// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalDetailSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UPortalDetailSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetParentWidget(class UPortalDetailWidget* NewParentWidget);

	UFUNCTION()
	void ClickedPortalSlot();

	FORCEINLINE class UButton* GetSlotBt() { return SlotBt; }
	FORCEINLINE class UTexture2D* GetLevelImg() { return LevelImage; }
	FORCEINLINE class FString GetDetail() { return Detail; }
	FORCEINLINE FVector GetPortLocation() { return PortLocation; }
	FORCEINLINE FRotator GetPortRotation() { return PortRotation; }

private:
	UPROPERTY()
	UPortalDetailWidget* ParentWidget;

	UPROPERTY()
	UButton* SlotBt;

	UPROPERTY()
	UTextBlock* PortLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FString Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UTexture2D* LevelImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FString PortLevelNameTxt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FVector PortLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FRotator PortRotation;
};
