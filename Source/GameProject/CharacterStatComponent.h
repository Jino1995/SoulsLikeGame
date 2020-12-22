// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLevelChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnMaxHealthChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCurrentHealthChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCurrentHealthZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMaxStaminaChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCurrentStaminaChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCurrentStaminaZeroDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDefenseChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnHPStatChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSPStatChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSTRStatChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAGIStatChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCONStatChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnStatPointChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void LevelUp();
	void IncHP(int32 IncPoint);
	void IncSP(int32 IncPoint);
	void IncSTR(int32 IncPoint);
	void IncAGI(int32 IncPoint);
	void IncCON(int32 IncPoint);

	void DecHP(int32 DecPoint);
	void DecSP(int32 DecPoint);
	void DecSTR(int32 DecPoint);
	void DecAGI(int32 DecPoint);
	void DecCON(int32 DecPoint);

	void IncMaxHealth(float IncFigure);
	int32 IncCurrentHealth(float IncFigure);
	void IncMaxStamina(float IncFigure);
	void IncCurrentStamina(float IncFigure);

	void IncAttack(int32 IncFigure);
	void IncDefense(int32 IncFigure);
	void IncSprintSpeed(int32 IncFigure);

	void DecMaxHealth(float DecFigure);
	void DecCurrentHealth(float DecFigure);
	void DecMaxStamina(float DecFigure);
	bool DecCurrentStamina(float DecFigure);
	void DecAttack(int32 DecFigure);
	void DecDefense(int32 DecFigure);
	void DecSprintSpeed(int32 DecFigure);

	bool DecStaminaForEva(float DecFigure);

	void IncStatForItem(FName ItemName);
	void DecStatForItem(FName ItemName);

	void UseStatPointForHP();
	void UseStatPointForSP();
	void UseStatPointForSTR();
	void UseStatPointForAGI();

public:
	void InitStat();
	void SetCurrentSPToZero();

	void SaveCharacterStatInfo(class UABSaveGame* SaveInstance);
	void LoadCharacterStatInfo(class UABSaveGame* LoadInstance);

	/**
	* 스탯 Setter 메서드
	*/
	FORCEINLINE int32 GetLevel() { return Level; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetCurrentStamina() { return CurrentStamina; }

	FORCEINLINE int32 GetAttack() { return Attack; }
	FORCEINLINE int32 GetDefense() { return Defense; }

	FORCEINLINE int32 GetHPStat() { return HP; }
	FORCEINLINE int32 GetSPStat() { return SP; }
	FORCEINLINE int32 GetSTRStat() { return STR; }
	FORCEINLINE int32 GetAGIStat() { return AGI; }
	FORCEINLINE int32 GetCONStat() { return CON; }
	FORCEINLINE int32 GetStatPoint() { return StatPoint; }

	FORCEINLINE float GetNormalSpeed() { return NormalSpeed; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE float GetMaxWeightPerOneCON() { return MaxWeightPerOneCON; }

	FORCEINLINE int32 GetBaseHP() { return BaseHP; }
	FORCEINLINE int32 GetBaseSP() { return BaseSP; }
	FORCEINLINE int32 GetBaseSTR() { return BaseSTR; }
	FORCEINLINE int32 GetBaseAGI() { return BaseAGI; }
	FORCEINLINE int32 GetBaseCON() { return BaseCON; }
	//
public:
	FOnLevelChangedDelegate OnLevelChanged;
	FOnMaxHealthChangedDelegate OnMaxHealthChanged;
	FOnCurrentHealthChangedDelegate OnCurrentHealthChanged;
	FOnCurrentHealthZeroDelegate OnCurrentHealthZero;
	FOnMaxStaminaChangedDelegate OnMaxStaminaChanged;
	FOnCurrentStaminaChangedDelegate OnCurrentStaminaChanged;
	FOnCurrentStaminaZeroDelegate OnCurrentStaminaZero;
	
	FOnAttackChangedDelegate OnAttackChanged;
	FOnDefenseChangedDelegate OnDefenseChanged;

	FOnHPStatChangedDelegate OnHPStatChanged;
	FOnSPStatChangedDelegate OnSPStatChanged;
	FOnSTRStatChangedDelegate OnSTRStatChanged;
	FOnAGIStatChangedDelegate OnAGIStatChanged;
	FOnCONStatChangedDelegate OnCONStatChanged;

	FOnStatPointChangedDelegate OnStatPointChanged;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 Level;

	/**
	* 플레이어가 올릴 수 있는 스탯
	* HP : MaxHealth 증가
	* SP : MaxStamina 증가
	* STR : Attack 증가
	* AGI : Speed 증가, Jump/Roll 행동 시 소모되는 Stamina 감소
	* CON : MaxWeight 증가
	* StatPoint : 사용 가능 스탯 포인트
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 HP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 SP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 STR;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 AGI;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 CON;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 StatPoint;
	//

	/**
	* 플레이어 능력치
	* MaxHealth : 최대 Health
	* MaxStamina : 최대 Stamina
	* Attack : 공격력
	* Defense : 방어력
	* (Sprint | Normal | Walk)Speed : 이동속도
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float MaxHealth;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float MaxStamina;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float CurrentStamina;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 Attack;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 Defense;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float SprintSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float NormalSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float WalkSpeed;
	//

	/**
	* 플레이어 찍은 스탯당 증가 능력치
	* MaxHealthPerOneHP : 1 HP당 증가하는 MaxHealth
	* MaxStaminaPerOneHP : 1 SP당 증가하는 MaxStamina 
	* AttackPerOneSTR : 1 STR당 증가하는 Attack
	* SpeedPerOneAGI : 1 AGI당 증가하는 SprintSpeed
	* DecEvaPerOneAGI : 1 AGI당 감소하는 Jump/Roll 소모 Stamina
	* MaxWeightPerOneCON : 1 CON당 증가하는 MaxWeight
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float MaxHealthPerOneHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float MaxStaminaPerOneSP;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 AttackPerOneSTR;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 SpeedPerOneAGI;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	float DecEvaPerOneAGI;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxWeightPerOneCON;
	//

	/**
	* 플레이어가 스탯을 최대로 찍을 수 있는 수치
	* MaxLevel : 최대 레벨
	* MAXHP : 최대 HP
	* MAXSP : 최대 SP
	* MAXSTR : 최대 STR
	* MAXAGI : 최대 AGI
	* MAXCON : 최대 CON
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxLevel;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxHP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxSP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxSTR;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxAGI;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "MAX Player Stats", Meta = (AllowPrivateAccess = true))
	int32 MaxCON;
	//

	/** 
	* StaminaRate : 스태미나 자연 재생률 
	* ConsumeStaminaRate : 캐릭터 Sprint Stamina 소비율
	*/
	float StaminaRate;
	float ConsumeStaminaRate;
	//

	/** 베이스 스텟 */
	int32 BaseHP;
	int32 BaseSP;
	int32 BaseSTR;
	int32 BaseAGI;
	int32 BaseCON;
};
