// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemyAnimInstance.h"
#include "SkeletonEnemy.h"

USkeletonEnemyAnimInstance::USkeletonEnemyAnimInstance()
{
	CurrentSpeed = 0.0f;
}

void USkeletonEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
		ASkeletonEnemy* Character = Cast<ASkeletonEnemy>(Pawn);
		if (Character)
		{
			bDead = Character->IsDead();
		}
	}
}

void USkeletonEnemyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);

	int32 AttackNum = rand() % 2 + 1;
	FName AttackName = FName(*FString::Printf(TEXT("Attack%d"), AttackNum));
	Montage_JumpToSection(AttackName, AttackMontage);
}

void USkeletonEnemyAnimInstance::JumpToBlockedAttackSection()
{
	Montage_JumpToSection(FName("BlockedAttack"), AttackMontage);
}

void USkeletonEnemyAnimInstance::PlayTakeDamageMontage()
{
	Montage_Play(DamagedMontage, 1.0f);
}

void USkeletonEnemyAnimInstance::PlayDeadMontage() 
{
	Montage_Play(DamagedMontage, 1.0f);
	Montage_JumpToSection(FName("Dead"), DamagedMontage);
}

void USkeletonEnemyAnimInstance::AnimNotify_DamagedEnd()
{
	OnDamagedEnd.Broadcast();
}

void USkeletonEnemyAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void USkeletonEnemyAnimInstance::AnimNotify_AttackStart()
{
	OnAttackStart.Broadcast();
}

void USkeletonEnemyAnimInstance::AnimNotify_ActivateAttack()
{
	OnActivateAttack.Broadcast();
}

void USkeletonEnemyAnimInstance::AnimNotify_DeActivateAttack()
{
	OnDeActivateAttack.Broadcast();
}