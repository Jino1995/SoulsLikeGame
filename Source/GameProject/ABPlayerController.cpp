// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "HUDOverlay.h"
#include "PlayerCharacter.h"
#include "InventoryComponent.h"
#include "ABSaveGame.h"
#include "CharacterStatComponent.h"
#include "PauseMenu.h"
#include "ABInventoryWidget.h"
#include "ABCharacterStatWidget.h"
#include "PortalWidget.h"

AABPlayerController::AABPlayerController()
{
	bInventoryWindow = false;
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterStat = Cast<APlayerCharacter>(GetPawn())->GetCharacterStatComponent();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UHUDOverlay>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		HUDOverlay->BindCharacterStat(CharacterStat);
	}
	HUDOverlay->BindInventoryComp(Cast<APlayerCharacter>(GetPawn())->GetInventory());

	if (PauseMenuAsset)
	{
		PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuAsset);
		PauseMenu->AddToViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InventoryAsset)
	{
		InventoryWidget = CreateWidget<UABInventoryWidget>(this, InventoryAsset);
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CharacterStatAsset)
	{
		CharacterStatWidget = CreateWidget<UABCharacterStatWidget>(this, CharacterStatAsset);
		CharacterStatWidget->AddToViewport();
		CharacterStatWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (PortalWidgetAsset)
	{
		PortalWidget = CreateWidget<UPortalWidget>(this, PortalWidgetAsset);
		PortalWidget->AddToViewport();
		PortalWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	LoadGame();
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("ShowMouseCursor"), EInputEvent::IE_Pressed, this, &AABPlayerController::VisibleMouseCursor);
}

void AABPlayerController::RequestShowBossHPBarToHUD(class AEnemy* TargetBoss)
{
	if (bBossHPBar)
	{
		bBossHPBar = false;
		HUDOverlay->HiddenBossHPBarWidgetDelay();
	}
	else
	{
		bBossHPBar = true;
		HUDOverlay->VisibleBossHPBarWidget(TargetBoss);
	}
}

void AABPlayerController::VisibleInventoryWidget()
{
	if (!bInventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInputMode();
		SetPause(true);
		bInventoryWidget = true;
		bShowMouseCursor = true;
	}
}

void AABPlayerController::HiddenInventoryWidget()
{
	if (bInventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		SetGameInputMode();
		SetPause(false);
		bInventoryWidget = false;
		bShowMouseCursor = false;
	}
}

void AABPlayerController::VisibleCharacterStatWidget()
{
	if (!bCharacterStatWidget)
	{
		CharacterStatWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInputMode();
		SetPause(true);
		bCharacterStatWidget = true;
		bShowMouseCursor = true;
	}
}

void AABPlayerController::HiddenCharacterStatWidget()
{
	if (bCharacterStatWidget)
	{
		CharacterStatWidget->SetVisibility(ESlateVisibility::Hidden);
		SetGameInputMode();
		SetPause(false);
		bCharacterStatWidget = false;
		bShowMouseCursor = false;
	}
}

void AABPlayerController::VisibleMouseCursor()
{
	if (bShowMouseCursor)
	{
		bShowMouseCursor = false;
	}
	else
	{
		bShowMouseCursor = true;
	}
}

void AABPlayerController::VisiblePauseMenu()
{
	if (!bPauseMenu)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		SetUIInputMode();
		SetPause(true);
		bPauseMenu = true;
		bShowMouseCursor = true;
	}
}

void AABPlayerController::HiddenPauseMenu()
{
	if (bPauseMenu)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		SetGameInputMode();
		SetPause(false);
		bPauseMenu = false;
		bShowMouseCursor = false;
	}
}

void AABPlayerController::VisiblePortalWidget()
{
	if (!bPortalWidget)
	{
		PortalWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInputMode();
		SetPause(true);
		bPortalWidget = true;
		bShowMouseCursor = true;
	}
}

void AABPlayerController::HiddenPortalWidget()
{
	if (bPortalWidget)
	{
		PortalWidget->SetVisibility(ESlateVisibility::Hidden);
		SetGameInputMode();
		SetPause(false);
		bPortalWidget = false;
		bShowMouseCursor = false;
	}
}

void AABPlayerController::ChangeLevel(FName NewLevelName, FVector NewLocation, FRotator NewRotation)
{
	UABSaveGame* SaveGameInstance = NewObject<UABSaveGame>();
	
	SaveGameInstance->Location = NewLocation;
	SaveGameInstance->Rotation= NewRotation;

	Cast<APlayerCharacter>(GetPawn())->GetCharacterStat()->SaveCharacterStatInfo(SaveGameInstance);

	Cast<APlayerCharacter>(GetPawn())->GetInventory()->SaveCharacterItemInfo(SaveGameInstance);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, FString("ChangeLevel"), 0);
	UGameplayStatics::OpenLevel(GetWorld(), NewLevelName);
}

void AABPlayerController::LoadGame()
{
	auto ChangeLevelData = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("ChangeLevel"), 0));
	if (ChangeLevelData != nullptr)
	{
		Cast<APlayerCharacter>(GetPawn())->GetCharacterStat()->LoadCharacterStatInfo(ChangeLevelData);
		Cast<APlayerCharacter>(GetPawn())->GetInventory()->LoadCharacterItemInfo(ChangeLevelData);

		Cast<APlayerCharacter>(GetPawn())->SetActorRelativeLocation(ChangeLevelData->Location);
		Cast<APlayerCharacter>(GetPawn())->SetActorRelativeRotation(ChangeLevelData->Rotation);
	}
	else
	{
		Cast<APlayerCharacter>(GetPawn())->GetCharacterStat()->InitStat();
	}
	UGameplayStatics::DeleteGameInSlot(FString("ChangeLevel"), 0);
}