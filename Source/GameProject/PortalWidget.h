// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UPortalWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	bool OnClickedChapterButton(class UChapterButtonWidget* NewChapterButton);

private:
	UPROPERTY()
	class UHorizontalBox* ChapterButtons;

	UPROPERTY()
	class UBorder* ChapterDetailList;

	UPROPERTY()
	UChapterButtonWidget* CurrentChapterButton;

	UPROPERTY()
	class UPortalDetailWidget* CurrentPortDetailwidget;
	/** 
	* Chapter buttons
	*/
	/*
	UPROPERTY()
	class UButton* ShelterBt;

	UPROPERTY()
	UButton* Ch1Bt;

	UPROPERTY()
	UButton* Ch2Bt;

	UPROPERTY()
	UButton* Ch3Bt;

	UPROPERTY()
	UButton* Ch4Bt;

	UPROPERTY()
	UButton* Ch5Bt;

	UPROPERTY()
	UButton* Ch6Bt;
	//
	*/
	UPROPERTY()
	UButton* MoveBt;

	UPROPERTY()
	UButton* BackBt;

public:
	/**
	* Pressed chapter buttons
	*/
	/*
	UFUNCTION()
	void ClickedShelterBt();

	UFUNCTION()
	void ClickedCh1Bt();

	UFUNCTION()
	void ClickedCh2Bt();

	UFUNCTION()
	void ClickedCh3Bt();

	UFUNCTION()
	void ClickedCh4Bt();

	UFUNCTION()
	void ClickedCh5Bt();

	UFUNCTION()
	void ClickedCh6Bt();
	//
	*/
	UFUNCTION()
	void ClickedMoveBt();

	UFUNCTION()
	void ClickedBackBt();
};
