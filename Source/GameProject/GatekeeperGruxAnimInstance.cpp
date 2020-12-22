// Fill out your copyright notice in the Description page of Project Settings.


#include "GatekeeperGruxAnimInstance.h"
#include "GatekeeperGrux.h"

UGatekeeperGruxAnimInstance::UGatekeeperGruxAnimInstance()
{
	StrafeDirection = 0.f;
}

void UGatekeeperGruxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		AGatekeeperGrux* Character = Cast<AGatekeeperGrux>(Pawn);
		if (Character)
		{
			bDead = Character->IsDead();
			bJumping = Character->GetCharacterMovement()->IsFalling();
			CurrentSpeed = Character->GetVelocity().Size();
			StrafeDirection = Character->GetStrafeDirection();
			CurrentEMS = Character->GetEnemyMovementStatus();
		}
	}
}

void UGatekeeperGruxAnimInstance::PlayAttackMontage()
{
	Montage_Play(NormalAttackMontage, 1.0f);

	int32 AttackSectionNum = rand() % 2 + 1;
	FName AttackSectionName = FName(*FString::Printf(TEXT("Attack%d"), AttackSectionNum));
	Montage_JumpToSection(AttackSectionName, NormalAttackMontage);
}

void UGatekeeperGruxAnimInstance::PlayRoarAttackMontage()
{
	Montage_Play(RoarAttackMontage, 1.0f);
}

void UGatekeeperGruxAnimInstance::PlayTwinAttackMontage()
{
	Montage_Play(TwinAttackMontage, 1.0f);
}

void UGatekeeperGruxAnimInstance::PlayTwinRoarAttackMontage()
{
	Montage_Play(TwinRoarAttackMontage, 1.0f);
}

void UGatekeeperGruxAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_LeftWeaponOn()
{
	OnLeftWeaponOn.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_LeftWeaponOff()
{
	OnLeftWeaponOff.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_RightWeaponOn()
{
	OnRightWeaponOn.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_RightWeaponOff()
{
	OnRightWeaponOff.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_JumpAttackEnd()
{
	OnJumpAttackEnd.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_JumpAttack1HitCheck()
{
	OnJumpAttack1HitCheck.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_JumpAttack1End()
{
	OnJumpAttack1End.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_JumpAttack2HitCheck()
{
	OnJumpAttack2HitCheck.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_JumpAttack2End()
{
	OnJumpAttack2End.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_RoarAttackCheck()
{
	OnRoarAttackCheck.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_DisableRoarAttack()
{
	OnDisableRoarAttack.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_RoarEnd()
{
	OnRoarEnd.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_TwinAttackEnd()
{
	OnTwinAttackEnd.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_EnableTwinRoar()
{
	OnEnableTwinRoar.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_DisableTwinRoar()
{
	OnDisableTwinRoar.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_TwinRoarAttackEnd()
{
	OnTwinRoarAttackEnd.Broadcast();
}

void UGatekeeperGruxAnimInstance::AnimNotify_RotationOn()
{
	OnRotation.Broadcast();
}