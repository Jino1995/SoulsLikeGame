// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	Idle,
	Patrol,
	Damaged,
	MoveToTarget,
	Strafe,
	Attacking,
	Dead
};

UENUM(BlueprintType)
enum class EStrafeDirection:uint8
{
	Right,
	Left,
	Forward
};

UENUM(BlueprintType)
enum class EMonsterType :uint8
{
	Normal,
	Boss
};

UCLASS()
class GAMEPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UBossHPBar* BossHPBar;

	EEnemyMovementStatus CurrentEMS;
	EStrafeDirection StrafeDirection;

	float ShieldBlockDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	FName MonsterName;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void ShowTargetPoint(bool bShow) { TargetPointWidget->SetVisibility(bShow); }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }
	FORCEINLINE UParticleSystem* GetHitParticles() { return HitParticles; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return CurrentEMS; }
	FORCEINLINE float GetShieldBlockDamage() { return ShieldBlockDamage; }
	FORCEINLINE FName GetMonsterName() { return MonsterName; }
	FORCEINLINE void SetBossHPBar(class UBossHPBar* NewHPBar) { BossHPBar = NewHPBar; }
public:
	FOnHPChangedDelegate OnHPChanged;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* HitParticles;

	EMonsterType MonsterType;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* TargetPointWidget;

};
