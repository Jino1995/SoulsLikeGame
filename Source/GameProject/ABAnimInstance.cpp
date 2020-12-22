// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentSpeed = 0.f;

	JointTargetLeft = FVector(50.f, 217.50296f, -38.598057f);
	JointTargetRight = FVector(50.f, 217.f, 38.f);
}


void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();

		Character = Cast<APlayerCharacter>(Pawn);
		if (Character)
		{
			bJumping = Character->GetCharacterMovement()->IsFalling();

			bTargeting = Character->IsTargeting();

			Axis_X = Character->GetInputAxisValue("MoveForward") * Character->GetCharacterMovement()->MaxWalkSpeed;
			Axis_Y = Character->GetInputAxisValue("MoveRight") * Character->GetCharacterMovement()->MaxWalkSpeed;
		}
	}
}

void UABAnimInstance::JumpToAttackMontageSection(EWeaponTypes WeaponType, int32 NewSection)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_1H);	
		break;
	case EWeaponTypes::GreatSword:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage_GreatSW);
		break;
	default:
		break;
	}
}

void UABAnimInstance::JumpToAttackMontage2Section(EWeaponTypes WeaponType, int32 NewSection)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage2_1H);
		break;
	case EWeaponTypes::GreatSword:
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage2_GreatSW);
		break;
	default:
		break;
	}
}

void UABAnimInstance::JumpToWeaponBlockHitMontageSection(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_JumpToSection(FName("Block"), OneHandBlockingMontage);
		break;
	case EWeaponTypes::GreatSword:
		Montage_JumpToSection(FName("Block"), GreatSwordBlockingMontage);
		break;
	default:
		break;
	}
}

void UABAnimInstance::JumpToBlockHitMontageSection()
{
	Montage_JumpToSection(FName("Block"), ShieldBlockingMontage);
}

void UABAnimInstance::JumpToBlockBreakMontageSection()
{
	Montage_Play(ShieldBreakMontage, 1.0f);
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

FName UABAnimInstance::GetHitMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Hit%d"), Section));
}

void UABAnimInstance::PlayEquipingMontage(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Play(EquipingMontage_1H, 1.0f);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Play(EquipingMontage_GreatSW, 1.0f);
		break;
	default:
		break;
	}
}

void UABAnimInstance::PlayUnEquipingMontage(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Play(EquipingMontage_1H, 1.0f);
		Montage_JumpToSection(FName("UnEquip"), EquipingMontage_1H);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Play(EquipingMontage_GreatSW, 1.0f);
		Montage_JumpToSection(FName("UnEquip"), EquipingMontage_GreatSW);
		break;
	default:
		break;
	}

}

void UABAnimInstance::PlayRollingMontage()
{
	Montage_Play(RollingMontage, 1.0f);
}

void UABAnimInstance::PlayAttackMontage(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Play(AttackMontage_1H, 1.0f);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Play(AttackMontage_GreatSW, 1.0f);
		break;
	default:
		break;
	}
}

void UABAnimInstance::PlayAttackMontage2(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Play(AttackMontage2_1H, 1.0f);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Play(AttackMontage2_GreatSW, 1.0f);
		break;
	default:
		break;
	}
}

void UABAnimInstance::PlayDrinkPotionMontage()
{
	Montage_Play(DrinkPotionMontage, 1.0f);
}

void UABAnimInstance::PlayHitMontage()
{
	int32 RandomNum = rand() % 3 + 1;

	Montage_Play(HitMontage, 1.0f);
	Montage_JumpToSection(GetHitMontageSectionName(RandomNum), HitMontage);
}

void UABAnimInstance::PlayBlockingMontage()
{
	Montage_Play(ShieldBlockingMontage, 1.0f);
}

void UABAnimInstance::StopBlockingMontage()
{
	Montage_Stop(0.1f, ShieldBlockingMontage);
}

void UABAnimInstance::PlayWeaponBlockingMontage(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Play(OneHandBlockingMontage, 1.0f);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Play(GreatSwordBlockingMontage, 1.0f);
		break;
	default:
		break;
	}
}

void UABAnimInstance::StopWeaponBlockingMontage(EWeaponTypes WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponTypes::OneHandWeapon:
		Montage_Stop(0.1f, OneHandBlockingMontage);
		break;
	case EWeaponTypes::GreatSword:
		Montage_Stop(0.1f, GreatSwordBlockingMontage);
		break;
	default:
		break;
	}
}

void UABAnimInstance::PlayOpenBossDoorMontage()
{
	Montage_Play(InteractionMontage, 1.0f);
	Montage_JumpToSection(FName("OpenBossDoor"), InteractionMontage);
}

void UABAnimInstance::PlayCCMontages(float PlayRate)
{
	Montage_Play(CCMontages, PlayRate);
}

void UABAnimInstance::AnimNotify_JumpEnd()
{
	OnJumpEnd.Broadcast();
}

void UABAnimInstance::AnimNotify_ChangeToEquip()
{
	OnChangeToEquip.Broadcast();
}

void UABAnimInstance::AnimNotify_ChangeToUnEquip()
{
	OnChangeToUnEquip.Broadcast();
}

void UABAnimInstance::AnimNotify_EndEquiping()
{
	OnEndEquiping.Broadcast();
}

void UABAnimInstance::AnimNotify_EndUnEquiping()
{
	OnEndUnEquiping.Broadcast();
}

void UABAnimInstance::AnimNotify_EndRolling()
{
	OnEndRolling.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_EndAttack()
{
	OnEndAttack.Broadcast();
}

void UABAnimInstance::AnimNotify_Healing()
{
	OnHealing.Broadcast();
}

void UABAnimInstance::AnimNotify_EndDrinking()
{
	OnEndDrinking.Broadcast();
}


void UABAnimInstance::AnimNotify_ActivateCollision()
{
	OnActivateCollision.Broadcast();
}

void UABAnimInstance::AnimNotify_DeActivateCollision()
{
	OnDeActivateCollision.Broadcast();
}

void UABAnimInstance::AnimNotify_HitEnd()
{
	OnHitEnd.Broadcast();
}

void UABAnimInstance::AnimNotify_BlockEnd()
{
	OnBlockEnd.Broadcast();
}

void UABAnimInstance::AnimNotify_ShieldBreakEnd()
{
	OnShieldBreakEnd.Broadcast();
}

void UABAnimInstance::AnimNotify_EndOpenDoor()
{
	OnEndOpenDoor.Broadcast();
}

void UABAnimInstance::AnimNotify_StunEnd()
{
	OnStunEnd.Broadcast();
}

void UABAnimInstance::AnimNotify_PlaySwingSound()
{
	OnPlaySwingSound.Broadcast();
}

// 테스트 코드
void UABAnimInstance::SetLeftEffectorLocation(FVector NewEffortLocation)
{
	LeftFootEffectorLocation = NewEffortLocation;
}

void UABAnimInstance::SetRightEffectorLocation(FVector NewEffortLocation)
{
	RightFootEffectorLocation = NewEffortLocation;
}

void UABAnimInstance::SetLeftFootAlpha(float NewAlpha)
{
	LeftFootAlpha = NewAlpha;
}

void UABAnimInstance::SetRightFootAlpha(float NewAlpha)
{
	RightFootAlpha = NewAlpha;
}