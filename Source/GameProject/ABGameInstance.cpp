// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

UABGameInstance::UABGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemInfo(TEXT("/Game/DataTables/DT_ItemData.DT_ItemData"));
	if (DT_ItemInfo.Succeeded())
	{
		ItemTable = DT_ItemInfo.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Name : %s , DataTable loaded fail."), *GetName());
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelUpInfoInfo(TEXT("/Game/DataTables/DT_LevelUpInfo.DT_LevelUpInfo"));
	if (DT_LevelUpInfoInfo.Succeeded())
	{
		LevelUpInfoTable = DT_LevelUpInfoInfo.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Name : %s , DataTable loaded fail."), *GetName());
	}
}

FItemData* UABGameInstance::GetItemData(FName ItemName)
{
	return ItemTable->FindRow<FItemData>(ItemName, TEXT(""));
}

FLevelUpInfo* UABGameInstance::GetLevelUpInfo(int32 Level)
{
	return LevelUpInfoTable->FindRow<FLevelUpInfo>(*FString::FromInt(Level), TEXT(""));
}