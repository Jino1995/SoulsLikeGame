// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Enemy.h"
#include "AIController.h"
#include "SkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API ASkeletonEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkeletonEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	virtual void CombatRangeOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	bool DecHealth(float DecAmount);
	void IncHealth(float IncAmount);

	void Dead();

	FORCEINLINE bool IsDead() { return bDead; }

public:
	struct FPathFollowingRequestResult PatrolResult;

	FTimerHandle PatrolTimer;
	FTimerHandle MoveToTargetTimer;
	FTimerHandle AttackTimer;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Range", Meta = (AllowPrivateAccess = true))
	class USphereComponent* CombatRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* AttackCollision;

	UPROPERTY(VisibleAnywhere)
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* Sensing;

	UPROPERTY()
	class APlayerCharacter* CombatTarget;

	UPROPERTY()
	class USkeletonEnemyAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, Meta = (AllowPrivateAccess = true))
	float PatrolSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, Meta = (AllowPrivateAccess = true))
	float MoveToTargetSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float EnemyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrol, Meta = (AllowPrivateAccess = true))
	TArray<FVector> PatrolPoint;

	int32 CurrentPatrolNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrol, Meta = (AllowPrivateAccess = true))
	bool bPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrol, Meta = (AllowPrivateAccess = true))
	bool bDead;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* FloatingTextManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropItems, Meta = (AllowPrivateAccess = true))
	TMap<FName, int32> DropItems;

	bool bTargetOnAttackRange;
	bool bTargetOnCombatRange;
	bool bRotate;
	bool bAttacking;

private:
	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);

	void MoveToTarget();

	void PlayPatrol();

	void CheckTargetOnAttackRange();

	void Attack();

	void ItemDrop();

};
