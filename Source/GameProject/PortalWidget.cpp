// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "PortalDetailWidget.h"
#include "Engine.h"
#include "ABPlayerController.h"
#include "ChapterButtonWidget.h"
#include "PortalDetailSlot.h"

void UPortalWidget::NativeConstruct()
{
	ChapterButtons = Cast<UHorizontalBox>(GetWidgetFromName(FName("ChapterButtonBox")));
	for (auto ChapterSlot : ChapterButtons->GetAllChildren())
	{
		Cast<UChapterButtonWidget>(ChapterSlot)->SetParentWidget(this);
	}

	ChapterDetailList = Cast<UBorder>(GetWidgetFromName(FName("PortListPanel")));

	MoveBt = Cast<UButton>(GetWidgetFromName(FName("MoveButton")));
	MoveBt->OnClicked.AddDynamic(this, &UPortalWidget::ClickedMoveBt);

	BackBt = Cast<UButton>(GetWidgetFromName(FName("BackButton")));
	BackBt->OnClicked.AddDynamic(this, &UPortalWidget::ClickedBackBt);

	CurrentChapterButton = nullptr;
}

bool UPortalWidget::OnClickedChapterButton(UChapterButtonWidget* NewChapterButton)
{
	if (CurrentChapterButton)
	{
		if (CurrentChapterButton == NewChapterButton)
		{
			return false;
		}

		ChapterDetailList->RemoveChildAt(0);
		CurrentPortDetailwidget = CreateWidget<UPortalDetailWidget>(this, NewChapterButton->GetPortDetailWidgetAsset());
		ChapterDetailList->AddChild(CurrentPortDetailwidget);

		CurrentChapterButton->ResetWidgetState();
		CurrentChapterButton = NewChapterButton;

		return true;
	}
	else
	{
		CurrentChapterButton = NewChapterButton;
		CurrentPortDetailwidget = CreateWidget<UPortalDetailWidget>(this, NewChapterButton->GetPortDetailWidgetAsset());
		ChapterDetailList->AddChild(CurrentPortDetailwidget);
		return true;
	}
}

void UPortalWidget::ClickedMoveBt()
{
	if (CurrentChapterButton)
	{
		if (CurrentPortDetailwidget->GetPortTarget())
		{
			Cast<AABPlayerController>(GetOwningPlayer())->HiddenPortalWidget();
			Cast<AABPlayerController>(GetOwningPlayer())->ChangeLevel(
				CurrentChapterButton->GetPortLevelName(),
				CurrentPortDetailwidget->GetPortTarget()->GetPortLocation(),
				CurrentPortDetailwidget->GetPortTarget()->GetPortRotation()
			);
		}
	}
}

void UPortalWidget::ClickedBackBt()
{
	if (CurrentChapterButton)
	{
		CurrentChapterButton->ResetWidgetState();
		CurrentChapterButton = nullptr;
		ChapterDetailList->RemoveChildAt(0);
		CurrentPortDetailwidget = nullptr;
	}
	Cast<AABPlayerController>(GetOwningPlayer())->HiddenPortalWidget();
}