// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Enemy.h"
#include "Animation/AnimInstance.h"
#include "GatekeeperGruxAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLeftWeaponOnDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLeftWeaponOffDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRightWeaponOnDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRightWeaponOffDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRoarEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnJumpAttack1HitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpAttack1EndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpAttack2HitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpAttack2EndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnRoarAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDisableRoarAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRoarEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnTwinAttackEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnEnableTwinRoarDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDisableTwinRoarDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTwinRoarAttackEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnRotationOnDelegate);

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UGatekeeperGruxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UGatekeeperGruxAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAttackMontage();
	void PlayRoarAttackMontage();
	void PlayTwinAttackMontage();
	void PlayTwinRoarAttackMontage();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_LeftWeaponOn();
	UFUNCTION()
	void AnimNotify_LeftWeaponOff();

	UFUNCTION()
	void AnimNotify_RightWeaponOn();
	UFUNCTION()
	void AnimNotify_RightWeaponOff();

	UFUNCTION()
	void AnimNotify_JumpAttackEnd();

	UFUNCTION()
	void AnimNotify_JumpAttack1HitCheck();
	UFUNCTION()
	void AnimNotify_JumpAttack1End();
	UFUNCTION()
	void AnimNotify_JumpAttack2HitCheck();
	UFUNCTION()
	void AnimNotify_JumpAttack2End();

	UFUNCTION()
	void AnimNotify_RoarAttackCheck();
	UFUNCTION()
	void AnimNotify_DisableRoarAttack();
	UFUNCTION()
	void AnimNotify_RoarEnd();

	UFUNCTION()
	void AnimNotify_TwinAttackEnd();

	UFUNCTION()
	void AnimNotify_EnableTwinRoar();
	UFUNCTION()
	void AnimNotify_DisableTwinRoar();
	UFUNCTION()
	void AnimNotify_TwinRoarAttackEnd();

	UFUNCTION()
	void AnimNotify_RotationOn();

public:
	FOnAttackEndDelegate OnAttackEnd;
	FOnLeftWeaponOnDelegate OnLeftWeaponOn;
	FOnLeftWeaponOffDelegate OnLeftWeaponOff;
	FOnRightWeaponOnDelegate OnRightWeaponOn;
	FOnRightWeaponOffDelegate OnRightWeaponOff;
	FOnJumpAttackEndDelegate OnJumpAttackEnd;

	FOnJumpAttack1HitCheckDelegate OnJumpAttack1HitCheck;
	FOnJumpAttack1EndDelegate OnJumpAttack1End;
	FOnJumpAttack2HitCheckDelegate OnJumpAttack2HitCheck;
	FOnJumpAttack1EndDelegate OnJumpAttack2End;

	FOnRoarAttackCheckDelegate OnRoarAttackCheck;
	FOnDisableRoarAttackDelegate OnDisableRoarAttack;
	FOnRoarEndDelegate OnRoarEnd;

	FOnTwinAttackEndDelegate OnTwinAttackEnd;

	FOnEnableTwinRoarDelegate OnEnableTwinRoar;
	FOnDisableTwinRoarDelegate OnDisableTwinRoar;
	FOnTwinRoarAttackEndDelegate OnTwinRoarAttackEnd;

	FOnRotationOnDelegate OnRotation;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RoarAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwinAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TwinRoarAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EEnemyMovementStatus CurrentEMS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strafe, Meta = (AllowPrivateAccess = true))
	float StrafeDirection;

};
