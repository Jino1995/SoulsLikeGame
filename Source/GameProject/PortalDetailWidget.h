// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UPortalDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void ClickedPortListSlot(class UPortalDetailSlot* ClickedSlot);

	FORCEINLINE class UPortalDetailSlot* GetPortTarget() { return PortTarget; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UVerticalBox* PortalSlotList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UPortalDetailSlot* PortTarget;

	UPROPERTY()
	class UTextBlock* PortLevelDetail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UImage* LevelImage;

};
