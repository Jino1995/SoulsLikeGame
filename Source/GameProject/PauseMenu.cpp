// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "Engine.h"
#include "ABPlayerController.h"

void UPauseMenu::NativeConstruct()
{
	InventoryBt = Cast<UButton>(GetWidgetFromName(FName("InventoryButton")));
	InventoryBt->OnClicked.AddDynamic(this, &UPauseMenu::ClickedInventoryBt);

	StatBt = Cast<UButton>(GetWidgetFromName(FName("StatButton")));
	StatBt->OnClicked.AddDynamic(this, &UPauseMenu::ClickedStatBt);

	BackBt = Cast<UButton>(GetWidgetFromName(FName("BackButton")));
	BackBt->OnClicked.AddDynamic(this, &UPauseMenu::ClickedBackBt);

	MainMenuBt = Cast<UButton>(GetWidgetFromName(FName("MainMenuButton")));
	MainMenuBt->OnClicked.AddDynamic(this, &UPauseMenu::ClickedMainMenuBt);
}

void UPauseMenu::ClickedInventoryBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenPauseMenu();
	Cast<AABPlayerController>(GetOwningPlayer())->VisibleInventoryWidget();
}

void UPauseMenu::ClickedStatBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenPauseMenu();
	Cast<AABPlayerController>(GetOwningPlayer())->VisibleCharacterStatWidget();
}

void UPauseMenu::ClickedBackBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenPauseMenu();
}

void UPauseMenu::ClickedMainMenuBt()
{
	Cast<AABPlayerController>(GetOwningPlayer())->SetGameInputMode();
	UGameplayStatics::OpenLevel(GetWorld(), FName("TitleMap"));
}