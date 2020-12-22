// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SkeletonEnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDamagedEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnActivateAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeActivateAttackDelegate);
/**
 * 
 */
UCLASS()
class GAMEPROJECT_API USkeletonEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USkeletonEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAttackMontage();
	void JumpToBlockedAttackSection();

	void PlayTakeDamageMontage();
	void PlayDeadMontage();
	

public:
	FOnDamagedEndDelegate OnDamagedEnd;
	FOnAttackEndDelegate OnAttackEnd;
	FOnAttackStartDelegate OnAttackStart;
	FOnActivateAttackDelegate OnActivateAttack;
	FOnDeActivateAttackDelegate OnDeActivateAttack;

public:
	UFUNCTION()
	void AnimNotify_DamagedEnd();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_ActivateAttack();
	UFUNCTION()
	void AnimNotify_DeActivateAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DamagedMontage;

};

