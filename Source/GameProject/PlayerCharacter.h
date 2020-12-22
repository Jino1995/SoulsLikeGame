// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterBehavior :uint8
{
	Idle,
	Move,
	Equiping,
	Drinking,
	Interaction,
	Jump,
	Attack,
	Block,
	BlockHit,
	BlockBreak,
	Damaged,
	Stun,
	Dead
};

UENUM(BlueprintType)
enum class EMoveType :uint8
{
	Normal,
	Walk,
	Sprint
};

UENUM(BlueprintType)
enum class ECombatMode :uint8
{
	NonCombat,
	Combat
};

UCLASS()
class GAMEPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 컴포넌트 초기화 이후 - 액터의 컴포넌트 초기화 완료 후 호출 */
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	float TakeDamage_CanNotBlock(float DamageAmount);
public:
	/**
	* CheckStopRecoveryStamina : 스테미나 회복을 중지 할지 체크
	* CheckPlayConsumeStamina : 스테미나 소모를 시작할지(Sprint) 체크
	*/
	bool CheckStopRecoveryStamina();
	bool CheckPlayConsumeStamina();
	//

	class UCharacterStatComponent* GetCharacterStatComponent();

	bool CanMove(int32 AxisValue);

	void ReleasedLShift();
	void ReleasedLCtrl();

	void AddNewItem();

	void EquipWeapon(TSubclassOf<class AWeapon> NewWeapon);
	void EquipShield(TSubclassOf<class AShield> NewShield);

	void UnEquipWeapon();
	void UnEquipShield();

	void TargetingLockOn();

	UFUNCTION()
	virtual void TargetRangeOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void TargetRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CC_Stun(float PlayRate);

	void SwitchLevel(FName NewLevelName);

	FORCEINLINE ECombatMode GetbCombatMode() { return CombatMode; }
	FORCEINLINE class UInventoryComponent* GetInventory() { return Inventory; };
	FORCEINLINE class UCharacterStatComponent* GetCharacterStat() { return CharacterStat; }
	FORCEINLINE bool IsStrongAttack() { return bStrongAttack; }
	FORCEINLINE bool IsTargeting() { return bTargeting; }
	FORCEINLINE void SetInteractionTarget(class AInteractionActor* NewTarget) { InteractionTarget = NewTarget; }
	FORCEINLINE UParticleSystem* GetHitParticles() { return HitParticles; }
	FORCEINLINE bool SetbBossBattle(bool Value) { return bBossBattle = Value; }
	FORCEINLINE bool IsBossBattle() { return bBossBattle; }

private:
	void SetPointOfView();
	void SetTargetingView();

	void MoveToForward(float NewAxisValue);
	void MoveToRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	void Attack();

	void AttackStartComboState();

	void AttackEndComboState();

	void PlayJump();

	void PressedLShift();
	void PressedLCtrl();

	void PlayEquiping();

	void PlayDrinkingPotion();

	bool SetCharacterRotationToAxis();

	void PlayBlocking();
	void StopBlocking();

	void PlayInteraction();

	void ShowPauseMenu();

	void TestCode();
	void TestLevelUp();
	void TestMoneyUp();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
		class UParticleSystem* HitParticles;

	UPROPERTY()
		AInteractionActor* InteractionTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting", Meta = (AllowPrivateAccess = true))
		class USphereComponent* TargetRange;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* FloatingTextManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion", Meta = (AllowPrivateAccess = true))
		TSubclassOf<class APotion> Potion_BP;

	UPROPERTY()
		class APotion* Potion;

	UPROPERTY()
		USpringArmComponent* SpringArm;

	UPROPERTY()
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = true))
		class AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Meta = (AllowPrivateAccess = true))
		class AShield* CurrentShield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
		class UCharacterStatComponent* CharacterStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats", Meta = (AllowPrivateAccess = true))
		class UInventoryComponent* Inventory;

	UPROPERTY()
		class UABAnimInstance* ABAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior", Meta = (AllowPrivateAccess = true))
		ECharacterBehavior CurrentBehavior;

	EMoveType CurrentMoveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatMode", Meta = (AllowPrivateAccess = true))
		ECombatMode CombatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina Consume", Meta = (AllowPrivateAccess = true))
		float BaseEvaStaminaConsumption;

	UPROPERTY()
		class AEnemy* TargetingTarget;

	TArray<class AEnemy*> TargetingEnemys;

	bool bMoveingForward;
	bool bMoveingRight;

	bool bPressedLShift;
	bool bPressedLCtrl;
	bool bPressedRMouse;

	bool bEquiping;

	int32 CurrentCombo;
	int32 MaxCombo;
	bool bComboInputOn;
	bool bNextAttack;

	bool bStrongAttack;

	bool bTargeting;

	bool bBossBattle;

// IK 테스트
protected:
	/** Left foot bone name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK, Meta=(BlueprintProtected = true))
	FName LeftFootBoneName;

	/** Right foot bone name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IK, Meta = (BlueprintProtected = true))
	FName RightFootBoneName;

	/** Right foot bone relative location */
	FVector LeftFootBoneRelativeLocation;

	/** Left foot bone relative location */
	FVector RightFootBoneRelativeLocation;

	/** Intial Skeletal Mesh position */
	FVector InitialMeshRelativeLocation;

};