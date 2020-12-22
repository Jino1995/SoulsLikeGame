// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDOverlay.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UHUDOverlay : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void BindCharacterStat(class UCharacterStatComponent* StatComp);
	void BindInventoryComp(class UInventoryComponent* InvenComp);

	void VisibleBossHPBarWidget(class AEnemy* TargetBoss);
	void HiddenBossHPBarWidgetDelay();
	void HiddenBossHPBarWidget();

public:
	FTimerHandle DelayTimer;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	class UCharacterStateWidget* StateWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	class USystemTextContainerWidget* SystemTextContainerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	class UPotionSlot* PotionSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	class UBossHPBar* BossHPBarWidget;
};
