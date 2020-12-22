// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "InventoryComponent.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Level;
	UPROPERTY()
	float MaxHealth;
	UPROPERTY()
	float CurrentHealth;
	UPROPERTY()
	float MaxStamina;
	UPROPERTY()
	float CurrentStamina;
	UPROPERTY()
	int32 HP;
	UPROPERTY()
	int32 SP;
	UPROPERTY()
	int32 STR;
	UPROPERTY()
	int32 AGI;
	UPROPERTY()
	int32 CON;
	UPROPERTY()
	int32 StatPoint;

	UPROPERTY()
	int32 Money;

	UPROPERTY()
	FName LevelName;

	UPROPERTY()
	FVector Location;
	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	TArray<FInventorySlotData> Items;

	UPROPERTY()
	TMap<EEquipmentType, FName> Equipments;

};
