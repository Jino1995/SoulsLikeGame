// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "ABGameInstance.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Etc,
	Material,
	Equipment
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	Head,
	Chest,
	Legs,
	Weapon,
	Shield
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	OneHanded,
	TwoHanded
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() : Name(FName("Default")), Icon(nullptr), ItemType(EItemType::None), IsStackable(false), MaxStackSize(0), Weapon_BP(nullptr), Shield_BP(nullptr), EquipmentType(EEquipmentType::None), WeaponType(EWeaponType::None), WeaponDetail(FString("None")), Damage(0), Armor(0), Description(FString("None")), HP(0), SP(0), STR(0), AGI(0), CON(0), Purchase(0), Sale(0), Weight(0) {}

	FName GetName() { return Name; }
	UTexture2D* GetIcon() { return Icon; }
	EItemType GetItemType() { return ItemType; }
	EEquipmentType GetEquipmentType() { return EquipmentType; }
	bool GetIsStackable() { return IsStackable; }
	int32 GetMaxStackSize() { return MaxStackSize; }
	FString GetDescription() { return Description; }
	EWeaponType GetWeaponType() { return WeaponType; }
	TSubclassOf<class AWeapon> GetWeaponBP() { return Weapon_BP; }
	TSubclassOf<class AShield> GetShieldBP() { return Shield_BP; }
	int32 GetDamage() { return Damage; }
	int32 GetArmor() { return Armor; }
	int32 GetHP() { return HP; }
	int32 GetSP() { return SP; }
	int32 GetSTR() { return STR; }
	int32 GetAGI() { return AGI; }
	int32 GetCON() { return CON; }
	int32 GetSale() { return Sale; }
	int32 GetWeight() { return Weight; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	bool IsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> Weapon_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AShield> Shield_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	FString WeaponDetail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 STR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 AGI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 CON;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Purchase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Sale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Weight;
};

USTRUCT(BlueprintType)
struct FLevelUpInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLevelUpInfo() : Level(1), LevelUpCost(100) {}

	int32 GetLevel() { return Level; }

	int32 GetLevelUpCost() { return LevelUpCost; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	int32 LevelUpCost;
};

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UABGameInstance();

	FItemData* GetItemData(FName ItemName);

	FLevelUpInfo* GetLevelUpInfo(int32 Level);

private:
	UPROPERTY()
	class UDataTable* ItemTable;

	UPROPERTY()
	class UDataTable* LevelUpInfoTable;
};
