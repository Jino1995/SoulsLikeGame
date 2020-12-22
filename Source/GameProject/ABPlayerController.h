// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

public:
	void RequestShowBossHPBarToHUD(class AEnemy* TargetBoss);

public:
	void VisibleInventoryWidget();
	void HiddenInventoryWidget();

	void VisibleCharacterStatWidget();
	void HiddenCharacterStatWidget();

	void VisibleMouseCursor();

	void VisiblePauseMenu();
	void HiddenPauseMenu();

	void VisiblePortalWidget();
	void HiddenPortalWidget();

public:
	void ChangeLevel(FName NewLevelName, FVector NewLocation, FRotator NewRotation);

	void LoadGame();

	FORCEINLINE void SetUIInputMode() { SetInputMode(UIInputMode); }

	FORCEINLINE void SetGameInputMode() { SetInputMode(GameInputMode); }

	FORCEINLINE class UHUDOverlay* GetHUDOverlay() { return HUDOverlay; }

	FORCEINLINE class UABInventoryWidget* GetInventoryWidget() { return InventoryWidget; }

private:
	UPROPERTY()
	class UCharacterStatComponent* CharacterStat;

	/** HUD Overlay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UHUDOverlay> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	UHUDOverlay* HUDOverlay;
	//

	/** Pause Menu */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPauseMenu> PauseMenuAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	UPauseMenu* PauseMenu;
	//

	/** Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UABInventoryWidget> InventoryAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	UABInventoryWidget* InventoryWidget;
	//

	/** Character Stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UABCharacterStatWidget> CharacterStatAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	UABCharacterStatWidget* CharacterStatWidget;
	//

	/** PortalWidget Stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPortalWidget> PortalWidgetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", Meta = (AllowPrivateAccess = true))
	UPortalWidget* PortalWidget;
	//

	bool bInventoryWindow;
	bool bInventoryWidget;
	bool bCharacterStatWidget;
	bool bPauseMenu;
	bool bPortalWidget;
	bool bBossHPBar;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
