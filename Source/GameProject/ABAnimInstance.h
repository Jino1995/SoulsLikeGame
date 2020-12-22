// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "ABAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnJumpEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChangeToEquipDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChangeToUnEquipDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndEquipingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndUnEquipingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndRollingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHealingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndDrinkingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnActivateCollision);
DECLARE_MULTICAST_DELEGATE(FOnDeActivateCollision);
DECLARE_MULTICAST_DELEGATE(FOnHitEndCollision);
DECLARE_MULTICAST_DELEGATE(FOnBlockEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnShieldBreakEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndOpenDoorDelegate);

DECLARE_MULTICAST_DELEGATE(FOnStunEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnPlaySwingSoundDelegate);

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void JumpToAttackMontageSection(EWeaponTypes WeaponType, int32 NewSection);

	void JumpToAttackMontage2Section(EWeaponTypes WeaponType, int32 NewSection);

	void JumpToWeaponBlockHitMontageSection(EWeaponTypes WeaponType);
	void JumpToBlockHitMontageSection();
	void JumpToBlockBreakMontageSection();

	FName GetAttackMontageSectionName(int32 Section);

	FName GetHitMontageSectionName(int32 Section);

public:
	void PlayEquipingMontage(EWeaponTypes WeaponType);
	void PlayUnEquipingMontage(EWeaponTypes WeaponType);

	void PlayRollingMontage();

	void PlayAttackMontage(EWeaponTypes WeaponType);
	void PlayAttackMontage2(EWeaponTypes WeaponType);

	void PlayDrinkPotionMontage();

	void PlayHitMontage();

	void PlayBlockingMontage();
	void StopBlockingMontage();

	void PlayWeaponBlockingMontage(EWeaponTypes WeaponType);
	void StopWeaponBlockingMontage(EWeaponTypes WeaponType);

	void PlayOpenBossDoorMontage();

	void PlayCCMontages(float PlayRate);

public:
	UFUNCTION()
	void AnimNotify_JumpEnd();

	UFUNCTION()
	void AnimNotify_ChangeToEquip();
	UFUNCTION()
	void AnimNotify_ChangeToUnEquip();
	UFUNCTION()
	void AnimNotify_EndEquiping();
	UFUNCTION()
	void AnimNotify_EndUnEquiping();

	UFUNCTION()
	void AnimNotify_EndRolling();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	UFUNCTION()
	void AnimNotify_EndAttack();

	UFUNCTION()
	void AnimNotify_Healing();
	UFUNCTION()
	void AnimNotify_EndDrinking();

	UFUNCTION()
	void AnimNotify_ActivateCollision();
	UFUNCTION()
	void AnimNotify_DeActivateCollision();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_BlockEnd();

	UFUNCTION()
	void AnimNotify_ShieldBreakEnd();

	UFUNCTION()
	void AnimNotify_EndOpenDoor();

	UFUNCTION()
	void AnimNotify_StunEnd();

	UFUNCTION()
	void AnimNotify_PlaySwingSound();

public:
	FOnJumpEndDelegate OnJumpEnd;
	FOnChangeToEquipDelegate OnChangeToEquip;
	FOnChangeToEquipDelegate OnChangeToUnEquip;
	FOnEndEquipingDelegate OnEndEquiping;
	FOnEndUnEquipingDelegate OnEndUnEquiping;
	FOnEndRollingDelegate OnEndRolling;
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnEndAttackDelegate OnEndAttack;
	FOnEndDrinkingDelegate OnEndDrinking;
	FOnHealingDelegate OnHealing;
	FOnActivateCollision OnActivateCollision;
	FOnDeActivateCollision OnDeActivateCollision;
	FOnHitEndCollision OnHitEnd;
	FOnBlockEndDelegate OnBlockEnd;
	FOnShieldBreakEndDelegate OnShieldBreakEnd;
	FOnEndOpenDoorDelegate OnEndOpenDoor;

	FOnStunEndDelegate OnStunEnd;

	FOnPlaySwingSoundDelegate OnPlaySwingSound;
private:
	/**
	* 1H Montages
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1HMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_1H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1HMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage2_1H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1HMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipingMontage_1H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* OneHandBlockingMontage;
	//
	
	/**
	* Great Sword Montages
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage_GreatSW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage2_GreatSW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword", Meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipingMontage_GreatSW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* GreatSwordBlockingMontage;
	//

	/**
	* Shield Montage
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ShieldBlockingMontage;
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* RollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DrinkPotionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ShieldBreakMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* InteractionMontage;
	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseMontage", Meta = (AllowPrivateAccess = true))
	UAnimMontage* CCMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	class APlayerCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float Axis_X;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float Axis_Y;

protected:
	/** Left foot effector location */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	FVector LeftFootEffectorLocation;

	/** Join target left */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	FVector JointTargetLeft;

	/** Left foot alpha */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	float LeftFootAlpha;

	/** Right foot effector location */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	FVector RightFootEffectorLocation;

	/** Join target right */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	FVector JointTargetRight;

	/** Right foot alpha */
	UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = IK, Meta = (BlueprintProtected = true))
	float RightFootAlpha;

public:
	void SetLeftEffectorLocation(FVector NewEffortLocation);

	void SetRightEffectorLocation(FVector NewEffortLocation);

	void SetLeftFootAlpha(float NewAlpha);

	void SetRightFootAlpha(float NewAlpha);
};
