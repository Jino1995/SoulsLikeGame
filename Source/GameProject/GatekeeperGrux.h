// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Enemy.h"
#include "GatekeeperGrux.generated.h"

UENUM(BlueprintType)
enum class EWeaponAttackType : uint8
{
	NormalAttack,
	TwinAttack,
	TwinRoarAttack
};

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AGatekeeperGrux : public AEnemy
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AGatekeeperGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void JumpAttackCollision1OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void JumpAttackCollision1OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void JumpAttackCollision2OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void JumpAttackCollision2OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void RoarAttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void RoarAttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void TwinRoarAttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void TwinRoarAttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void EnableLeftWeaponTrace();

	void DisableLeftWeaponTrace();

	void EnableRightWeaponTrace();

	void DisableRightWeaponTrace();

	void EnableJumpAttack();
	void EnableRoarAttack();
	void EnableTwinAttack();
	void EnableTwinRoarAttack();

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);

	UFUNCTION()
	void PlayStrafing();

	void CheckDistanceToEnemy();
	
	void MoveToTarget();

	void AttackSelect();

	void PlayNormalAttack();
	
	void PlayJumpAttack();

	void PlayRoarAttack();

	void PlayTwinAttack();

	void PlayTwinRoarAttack();

	bool DecHealth(float DecAmount);

	void Dead();

	FORCEINLINE int32 GetStrafeDirection() { return StrafeDirectionValue; }
	FORCEINLINE bool IsDead() { return bDead; }

public:
	FTimerHandle StrafingTimer;
	FTimerHandle CheckDistanceTimer;
	FTimerHandle AttackTimer;
	FTimerHandle JumpAttackCoolTimeTimer;
	FTimerHandle RoarAttackCoolTimeTimer;
	FTimerHandle TwinAttackCoolTimeTimer;
	FTimerHandle TwinRoarAttackCoolTimeTimer;
	FTimerHandle LeftWeaponTraceTimer;
	FTimerHandle RightWeaponTraceTimer;
	
	FTimerHandle DelayTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageTypeClass;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class USoundCue* ThemeMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class UAudioComponent* Music;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class UPhysicsAsset* DeadPhysicsAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* JumpAttackCollision1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* JumpAttackCollision2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class USphereComponent* RoarAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* TwinRoar_RoarCollision;

	UPROPERTY(VisibleAnywhere)
	class AAIController* AIController;

	
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* Sensing;
	
	UPROPERTY()
	class APlayerCharacter* CombatTarget;

	UPROPERTY()
	class UGatekeeperGruxAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MinAttackCheckTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MaxAttackCheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MinJumpAttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MaxJumpAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MinRoarAttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MaxRoarAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MinTwinAttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MaxTwinAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MinTwinRoarAttackCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer, Meta = (AllowPrivateAccess = true))
	float MaxTwinRoarAttackCoolTime;

	float StrafeDirectionValue;

	float DistanceToEnemy;

	TSet<class APlayerCharacter*> DamagedEnemy;

	bool bEnableJumpAttack;
	bool bEnableRoarAttack;
	bool bEnableTwinAttack;
	bool bEnableTwinRoarAttack;

	bool bDead;

	EWeaponAttackType WeaponAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float NormalAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float TwinAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float RoarAttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float TwinRoarAttack_TwinDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float TwinRoarAttack_RoarDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float JumpAttack1Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float JumpAttack2Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CC, Meta = (AllowPrivateAccess = true))
	float RoarAttackStunRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CC, Meta = (AllowPrivateAccess = true))
	float TwinRoarAttack_TwinStunRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CC, Meta = (AllowPrivateAccess = true))
	float TwinRoarAttack_RoarStunRate;
};