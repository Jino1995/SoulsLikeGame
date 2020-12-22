// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "CharacterStatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ABAnimInstance.h"
#include "ABPlayerController.h"
#include "InventoryComponent.h"
#include "Weapon.h"
#include "Shield.h"
#include "Engine.h"
#include "Potion.h"
#include "Enemy.h"
#include "Components/WidgetComponent.h"
#include "HPFloatingText.h"
#include "InteractionActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Game/AshenKnight/Mesh/SK_AshenKnight.SK_AshenKnight"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed load to player's SK_Mesh"));
	}
	
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_Anim(TEXT("/Game/AshenKnight/Blueprint/BP_AshenKnight_ANIM.BP_AshenKnight_ANIM_C"));
	if (BP_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_Anim.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("INVENTORY"));

	TargetRange = CreateDefaultSubobject<USphereComponent>(TEXT("TARGETRANGE"));
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->InitSphereRadius(800.0f);

	SetPointOfView();

	CurrentBehavior = ECharacterBehavior::Idle;
	CurrentMoveType = EMoveType::Normal;

	bMoveingForward = false;
	bMoveingRight = false;

	bPressedLShift = false;

	bEquiping = false;

	bTargeting = false;

	MaxCombo = 2;
	AttackEndComboState();
	
	CombatMode = ECombatMode::NonCombat;

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	FloatingTextManager = CreateDefaultSubobject<UWidgetComponent>(TEXT("DAMAGETEXT"));
	FloatingTextManager->SetupAttachment(GetMesh());
	FloatingTextManager->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	FloatingTextManager->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> DamageText_Widget(TEXT("/Game/HUD/FloatingText/BP_FloatingText.BP_FloatingText_C"));
	if (DamageText_Widget.Succeeded())
	{
		FloatingTextManager->SetWidgetClass(DamageText_Widget.Class);
		FloatingTextManager->SetDrawSize(FVector2D(10.0f, 10.0f));
		FloatingTextManager->SetVisibility(true);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::TargetRangeOnOverlapBegin);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::TargetRangeOnOverlapEnd);
	TargetRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	TargetRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// 테스트
	if (GetMesh()->SkeletalMesh)
	{
		const FVector LeftFootBoneWorldLocation = GetMesh()->GetBoneLocation(LeftFootBoneName);
		LeftFootBoneRelativeLocation = GetActorTransform().InverseTransformPosition(LeftFootBoneWorldLocation);

		const FVector RightFootBoneWorldLocation = GetMesh()->GetBoneLocation(RightFootBoneName);
		RightFootBoneRelativeLocation = GetActorTransform().InverseTransformPosition(RightFootBoneWorldLocation);

		InitialMeshRelativeLocation = GetMesh()->GetRelativeTransform().GetLocation();
	}
	//
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentBehavior == ECharacterBehavior::Idle || CurrentBehavior == ECharacterBehavior::Move || CurrentBehavior == ECharacterBehavior::Block)
	{
		if (bPressedLShift && (bMoveingForward || bMoveingRight) && CurrentBehavior != ECharacterBehavior::Block)
		{
			CurrentBehavior = ECharacterBehavior::Move;
			CurrentMoveType = EMoveType::Sprint;
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetSprintSpeed();
		}
		else if (bPressedLCtrl && (bMoveingForward || bMoveingRight))
		{
			if (CurrentBehavior != ECharacterBehavior::Block) CurrentBehavior = ECharacterBehavior::Move;
			CurrentMoveType = EMoveType::Walk;
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetWalkSpeed();
		}
		else if (bMoveingForward || bMoveingRight)
		{
			if (CurrentBehavior != ECharacterBehavior::Block) CurrentBehavior = ECharacterBehavior::Move;
			CurrentMoveType = EMoveType::Normal;
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetNormalSpeed();
		}
		else
		{
			if (CurrentBehavior != ECharacterBehavior::Block) CurrentBehavior = ECharacterBehavior::Idle;
			CurrentMoveType = EMoveType::Normal;
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetNormalSpeed();
		}
	}

	if (bTargeting)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetingTarget->GetActorLocation());
		FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotationYaw, DeltaTime, 15.0f);
		SetActorRotation(InterpRotation);

		GetController()->SetControlRotation(FRotator(-45.f, InterpRotation.Yaw, 0.f));
	}

	if (GetVelocity().Size() == 0)
	{
		TArray<AActor*> ActorsToIgnore;

		FHitResult LeftTraceHit;
		const FVector LeftFootLocation = GetTransform().TransformPosition(LeftFootBoneRelativeLocation);
		bool bLeftFootTraceHit = UKismetSystemLibrary::SphereTraceSingle(this, LeftFootLocation + FVector(0.f, 0.f, 50.f), LeftFootLocation + FVector(0.f, 0.f, -100.f), 10, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, LeftTraceHit, true);
		
		FHitResult RightTraceHit;
		const FVector RightFootLocation = GetTransform().TransformPosition(RightFootBoneRelativeLocation);
		bool bRightFootTraceHit = UKismetSystemLibrary::SphereTraceSingle(this, RightFootLocation + FVector(0.f, 0.f, 50.f), RightFootLocation + FVector(0.f, 0.f, -100.f), 10, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, RightTraceHit, true);

		float ZOffset = 0.f;
		if (LeftTraceHit.ImpactPoint.Z < RightTraceHit.ImpactPoint.Z)
		{
			ZOffset = LeftFootLocation.Z - LeftTraceHit.ImpactPoint.Z;
			ABAnim->SetRightEffectorLocation(RightTraceHit.ImpactPoint + FVector(0.f, 0.f, 5.f));
			ABAnim->SetLeftFootAlpha(0.f);
			ABAnim->SetRightFootAlpha(1.f);
		}
		else
		{
			ZOffset = RightFootLocation.Z - RightTraceHit.ImpactPoint.Z;
			ABAnim->SetLeftEffectorLocation(LeftTraceHit.ImpactPoint + FVector(0.f, 0.f, 5.f));
			ABAnim->SetLeftFootAlpha(1.f);
			ABAnim->SetRightFootAlpha(0.f);
		}
		GetMesh()->SetRelativeLocation(InitialMeshRelativeLocation + FVector(0.f, 0.f, -ZOffset + 5.f));
	}
	else
	{
		GetMesh()->SetRelativeLocation(InitialMeshRelativeLocation);
		ABAnim->SetLeftFootAlpha(0.f);
		ABAnim->SetRightFootAlpha(0.f);
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->OnCurrentStaminaZero.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
		CurrentMoveType = EMoveType::Walk;
		GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetWalkSpeed();
	});

	CharacterStat->OnCurrentHealthZero.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Dead;
	});

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

	ABAnim->OnJumpEnd.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
		CurrentMoveType = EMoveType::Walk;
		GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetWalkSpeed();
	});

	ABAnim->OnChangeToEquip.AddLambda([this]()->void {
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetEquipPosion());
		if (CurrentShield)
		{
			CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentShield->GetEquipPosion());
		}
		CombatMode = ECombatMode::Combat;
	});

	ABAnim->OnChangeToUnEquip.AddLambda([this]()->void {
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetUnEquipPosion());
		if (CurrentShield)
		{
			CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentShield->GetUnEquipPosion());
		}
		CombatMode = ECombatMode::NonCombat;
	});

	ABAnim->OnEndEquiping.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
	});

	ABAnim->OnEndUnEquiping.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
	});

	ABAnim->OnEndRolling.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
		if (TargetingTarget)
		{
			bTargeting = true;
		}
		if (bPressedRMouse)
		{
			PlayBlocking();
		}
	});

	ABAnim->OnEndAttack.AddLambda([this]()->void {
		if (bStrongAttack)
		{
			bStrongAttack = false;
		}
		CurrentBehavior = ECharacterBehavior::Idle;		
		AttackEndComboState();

		if (bPressedRMouse)
		{
			PlayBlocking();
		}
	});

	ABAnim->OnNextAttackCheck.AddLambda([this]()->void {
		bNextAttack = false;

		if (bComboInputOn)
		{
			AttackStartComboState();

			if (bPressedLShift && CharacterStat->GetCurrentStamina() - CurrentWeapon->GetStrongAttackStamina() >= 0)
			{
				ABAnim->JumpToAttackMontage2Section(CurrentWeapon->GetWeaponType(), CurrentCombo);
			}
			else if(!bPressedLShift && CharacterStat->GetCurrentStamina() - CurrentWeapon->GetAttackStamina() >= 0)
			{
				ABAnim->JumpToAttackMontageSection(CurrentWeapon->GetWeaponType(), CurrentCombo);
			}
		}
	});

	ABAnim->OnHealing.AddLambda([this]()->void {
		auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
		if (DWidget != nullptr)
		{
			float PotionHealingAmount = Inventory->GetPotionRecovery() / 100;
			int32 HealingAmount = CharacterStat->IncCurrentHealth(CharacterStat->GetMaxHealth() * PotionHealingAmount);
			DWidget->CreateHealingText(HealingAmount);
		}
	});

	ABAnim->OnEndDrinking.AddLambda([this]()->void {
		if (CurrentBehavior != ECharacterBehavior::Dead)
		{
			CurrentBehavior = ECharacterBehavior::Idle;
			Potion->Destroy();
		}
	});

	ABAnim->OnActivateCollision.AddLambda([this]() -> void {
		if (bStrongAttack)
		{
			CharacterStat->DecCurrentStamina(CurrentWeapon->GetStrongAttackStamina());
		}
		else
		{
			CharacterStat->DecCurrentStamina(CurrentWeapon->GetAttackStamina());
		}
		CurrentWeapon->ActivateCollision();
	});

	ABAnim->OnDeActivateCollision.AddLambda([this]() -> void {
		CurrentWeapon->DeActivateCollision();
	});

	ABAnim->OnHitEnd.AddLambda([this]()->void {
		if (CurrentBehavior != ECharacterBehavior::Dead)
		{
			CurrentBehavior = ECharacterBehavior::Idle;
			if (bPressedRMouse)
			{
				PlayBlocking();
			}
		}
	});

	ABAnim->OnBlockEnd.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
		if (bPressedRMouse)
		{
			PlayBlocking();
		}
	});

	ABAnim->OnShieldBreakEnd.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
	});

	ABAnim->OnEndOpenDoor.AddLambda([this]()->void {
		InteractionTarget->EndInteraction();
		CurrentBehavior = ECharacterBehavior::Idle;
	});

	ABAnim->OnStunEnd.AddLambda([this]()->void {
		CurrentBehavior = ECharacterBehavior::Idle;
	});

	ABAnim->OnPlaySwingSound.AddLambda([this]()->void {
		UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->GetSwingSound());
		
	});
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveToForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveToRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayJump);
	PlayerInputComponent->BindAction(TEXT("Equiping"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayEquiping);
	PlayerInputComponent->BindAction(TEXT("UsePotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayDrinkingPotion);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Pressed, this, &APlayerCharacter::TargetingLockOn);

	PlayerInputComponent->BindAction(TEXT("LShift"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PressedLShift);
	PlayerInputComponent->BindAction(TEXT("LShift"), EInputEvent::IE_Released, this, &APlayerCharacter::ReleasedLShift);
	PlayerInputComponent->BindAction(TEXT("LCtrl"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PressedLCtrl);
	PlayerInputComponent->BindAction(TEXT("LCtrl"), EInputEvent::IE_Released, this, &APlayerCharacter::ReleasedLCtrl);

	PlayerInputComponent->BindAction(TEXT("AddItem"), EInputEvent::IE_Pressed, this, &APlayerCharacter::AddNewItem);

	PlayerInputComponent->BindAction(TEXT("Blocking"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayBlocking);
	PlayerInputComponent->BindAction(TEXT("Blocking"), EInputEvent::IE_Released, this, &APlayerCharacter::StopBlocking);

	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayInteraction);

	PlayerInputComponent->BindAction(TEXT("PauseMenu"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ShowPauseMenu);

	PlayerInputComponent->BindAction(TEXT("One"), EInputEvent::IE_Pressed, this, &APlayerCharacter::TestMoneyUp);
	PlayerInputComponent->BindAction(TEXT("Two"), EInputEvent::IE_Pressed, this, &APlayerCharacter::TestLevelUp);
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if ((CurrentBehavior == ECharacterBehavior::Interaction) || (CurrentBehavior == ECharacterBehavior::Jump && CombatMode == ECombatMode::Combat))
	{
		return 0.f;
	}

	if (CurrentBehavior == ECharacterBehavior::Attack)
	{
		CurrentWeapon->DeActivateCollision();
	}

	if (IsValid(Potion))
	{
		Potion->Destroy();
	}

	AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
	if (Enemy)
	{
		if (CurrentBehavior == ECharacterBehavior::Block || CurrentBehavior == ECharacterBehavior::BlockHit)
		{
			FRotator BetweenAngle = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation());
			FRotator EnemyLocation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenAngle);
			bool bDamageZone = (UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, -180.0f, -70.0f) || UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, 70.0f, 180.0f));
			if (bDamageZone)
			{
				auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
				if (DWidget != nullptr)
				{
					DWidget->CreateDamageText(DamageAmount);
				}

				CurrentBehavior = ECharacterBehavior::Damaged;
				ABAnim->PlayHitMontage();
				CharacterStat->DecCurrentHealth(DamageAmount);
			}
			else
			{
				if (CharacterStat->DecCurrentStamina(Enemy->GetShieldBlockDamage()))
				{
					CurrentBehavior = ECharacterBehavior::BlockHit;
					if (IsValid(CurrentShield))
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentShield->GetBlockParticle(), CurrentShield->GetBlockSocket(), FRotator(0.0f), false);
						ABAnim->JumpToBlockHitMontageSection();
					}
					else
					{
						switch (CurrentWeapon->GetWeaponType())
						{
						case EWeaponTypes::OneHandWeapon:
							DamageAmount *= 0.5;
							break;
						case EWeaponTypes::GreatSword:
							DamageAmount *= 0.3;
							break;
						}
						auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
						if (DWidget != nullptr)
						{
							DWidget->CreateDamageText(DamageAmount);
						}
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->GetBlockParticle(), CurrentWeapon->GetBlockSocket(), FRotator(0.0f), false);
						ABAnim->JumpToWeaponBlockHitMontageSection(CurrentWeapon->GetWeaponType());
						CharacterStat->DecCurrentHealth(DamageAmount);
					}
					return -1.f;
				}
				else
				{
					CurrentBehavior = ECharacterBehavior::BlockBreak;
					if (IsValid(CurrentShield))
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentShield->GetBlockParticle(), CurrentShield->GetBlockSocket(), FRotator(0.0f), false);
						ABAnim->JumpToBlockBreakMontageSection();
					}
					else
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->GetBlockParticle(), CurrentWeapon->GetBlockSocket(), FRotator(0.0f), false);
						ABAnim->JumpToBlockBreakMontageSection();
					}
					return -2.f;
				}
			}
		}
		else
		{
			auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
			if (DWidget != nullptr)
			{
				DWidget->CreateDamageText(DamageAmount);
			}

			CurrentBehavior = ECharacterBehavior::Damaged;
			ABAnim->PlayHitMontage();
			CharacterStat->DecCurrentHealth(DamageAmount);
		}
	}

	return DamageAmount;
}

float APlayerCharacter::TakeDamage_CanNotBlock(float DamageAmount)
{
	if (CurrentBehavior == ECharacterBehavior::Interaction)
	{
		return 0.f;
	}

	if (IsValid(Potion))
	{
		Potion->Destroy();
	}

	auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
	if (DWidget != nullptr)
	{
		DWidget->CreateDamageText(DamageAmount);
	}

	CurrentBehavior = ECharacterBehavior::Damaged;
	ABAnim->PlayHitMontage();
	CharacterStat->DecCurrentHealth(DamageAmount);

	return DamageAmount;
}

void APlayerCharacter::SetPointOfView()
{
	SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 300.0f;

	SpringArm->bUsePawnControlRotation = true;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 4.0f;
	SpringArm->CameraRotationLagSpeed = 10.f;
	SpringArm->bDoCollisionTest = true;

	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);
}

void APlayerCharacter::SetTargetingView()
{
	SpringArm->bEnableCameraLag = false;
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 80.f, 80.f));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void APlayerCharacter::MoveToForward(float NewAxisValue)
{
	bMoveingForward = false;
	if (CanMove(NewAxisValue))
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		bMoveingForward = true;
	}
}

void APlayerCharacter::MoveToRight(float NewAxisValue)
{
	bMoveingRight = false;
	if (CanMove(NewAxisValue))
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		bMoveingRight = true;
	}
}

void APlayerCharacter::Turn(float NewAxisValue)
{
	if (!TargetingTarget)
	{
		AddControllerYawInput(NewAxisValue);
	}
}

void APlayerCharacter::LookUp(float NewAxisValue)
{
	if (!TargetingTarget)
	{
		AddControllerPitchInput(NewAxisValue);
	}
}

void APlayerCharacter::Attack()
{
	UE_LOG(LogTemp, Error, TEXT("Damage?? : %d"), GetCharacterStat()->GetAttack());
	if (CurrentBehavior == ECharacterBehavior::Jump || CurrentBehavior == ECharacterBehavior::Drinking || CurrentBehavior == ECharacterBehavior::Equiping || CurrentBehavior == ECharacterBehavior::Damaged || CurrentBehavior == ECharacterBehavior::Dead || CurrentBehavior == ECharacterBehavior::BlockHit || CurrentBehavior == ECharacterBehavior::BlockBreak || CurrentBehavior == ECharacterBehavior::Stun)
	{
		return;
	}

	if (CombatMode == ECombatMode::Combat)
	{
		if (CurrentBehavior == ECharacterBehavior::Attack)
		{
			if (bNextAttack && CharacterStat->GetCurrentStamina() - CurrentWeapon->GetAttackStamina() >= 0)
			{
				bComboInputOn = true;
			}
		}
		else
		{
			if (bPressedLShift && CurrentWeapon->GetStrongAttackStamina() <= CharacterStat->GetCurrentStamina())
			{
				CurrentBehavior = ECharacterBehavior::Attack;
				AttackStartComboState();
				bStrongAttack = true;
				ABAnim->PlayAttackMontage2(CurrentWeapon->GetWeaponType());
				ABAnim->JumpToAttackMontageSection(CurrentWeapon->GetWeaponType(), CurrentCombo);
			}
			else if (!bPressedLShift && CurrentWeapon->GetAttackStamina() <= CharacterStat->GetCurrentStamina())
			{
				CurrentBehavior = ECharacterBehavior::Attack;
				AttackStartComboState();
				ABAnim->PlayAttackMontage(CurrentWeapon->GetWeaponType());
				ABAnim->JumpToAttackMontageSection(CurrentWeapon->GetWeaponType(), CurrentCombo);
			}
		}
	}
	else
	{
		PlayEquiping();
	}
}

void APlayerCharacter::AttackStartComboState()
{
	bNextAttack = true;
	bComboInputOn = false;
	CurrentCombo = CurrentCombo % 2 + 1;
}

void APlayerCharacter::AttackEndComboState()
{
	bNextAttack = false;
	bComboInputOn = false;
	CurrentCombo = 0;
}

void APlayerCharacter::PlayJump()
{
	if (bEquiping || (CurrentBehavior != ECharacterBehavior::Idle && CurrentBehavior != ECharacterBehavior::Move && CurrentBehavior != ECharacterBehavior::Block))
	{
		return;
	}

	if (CharacterStat->DecStaminaForEva(BaseEvaStaminaConsumption))
	{
		switch (CombatMode)
		{
		case ECombatMode::NonCombat:
			CurrentBehavior = ECharacterBehavior::Jump;
			bPressedJump = true;
			JumpKeyHoldTime = 0.0f;
			break;
		case ECombatMode::Combat:
			if (TargetingTarget)
			{
				bTargeting = false;
			}
			CurrentBehavior = ECharacterBehavior::Jump;
			SetCharacterRotationToAxis();
			ABAnim->PlayRollingMontage();
			break;
		}
	}
}

bool APlayerCharacter::CheckStopRecoveryStamina()
{
	if (CurrentBehavior == ECharacterBehavior::Attack || CurrentBehavior == ECharacterBehavior::Jump || CurrentBehavior == ECharacterBehavior::Block || CurrentBehavior == ECharacterBehavior::BlockHit || CurrentBehavior == ECharacterBehavior::BlockBreak || CurrentBehavior == ECharacterBehavior::Damaged || CurrentBehavior == ECharacterBehavior::Stun)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::CheckPlayConsumeStamina()
{
	if (CurrentMoveType == EMoveType::Sprint)
	{
		return true;
	}
	else
	{
		return false;
	}
}

UCharacterStatComponent* APlayerCharacter::GetCharacterStatComponent()
{
	return CharacterStat;
}

bool APlayerCharacter::CanMove(int32 AxisValue)
{
	if (AxisValue != 0 && CurrentBehavior != ECharacterBehavior::Attack && CurrentBehavior != ECharacterBehavior::Jump && CurrentBehavior != ECharacterBehavior::Dead && CurrentBehavior != ECharacterBehavior::BlockHit && CurrentBehavior != ECharacterBehavior::BlockBreak && CurrentBehavior != ECharacterBehavior::Damaged && CurrentBehavior != ECharacterBehavior::Interaction && CurrentBehavior != ECharacterBehavior::Stun)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APlayerCharacter::PressedLShift()
{
	bPressedLShift = true;
}

void APlayerCharacter::ReleasedLShift()
{
	bPressedLShift = false;
}

void APlayerCharacter::PressedLCtrl()
{
	bPressedLCtrl = true;
}

void APlayerCharacter::ReleasedLCtrl()
{
	bPressedLCtrl = false;
}

void APlayerCharacter::PlayEquiping()
{
	if ((CurrentBehavior != ECharacterBehavior::Idle && CurrentBehavior != ECharacterBehavior::Move) || CurrentWeapon == nullptr || CurrentMoveType == EMoveType::Sprint)
	{
		return;
	}
	
	CurrentBehavior = ECharacterBehavior::Equiping;
	UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->GetEquippingSound());
	switch (CombatMode)
	{
	case ECombatMode::NonCombat:
		ABAnim->PlayEquipingMontage(CurrentWeapon->GetWeaponType());
		break;
	case ECombatMode::Combat:
		ABAnim->PlayUnEquipingMontage(CurrentWeapon->GetWeaponType());
		break;
	}
}

void APlayerCharacter::PlayDrinkingPotion()
{
	if (CurrentBehavior != ECharacterBehavior::Idle && CurrentBehavior != ECharacterBehavior::Move)
	{
		return;
	}
	else
	{
		if (Inventory->UsePotion())
		{
			CurrentBehavior = ECharacterBehavior::Drinking;
			CurrentMoveType = EMoveType::Walk;
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetWalkSpeed();
			Potion = GetWorld()->SpawnActor<APotion>(Potion_BP, FVector::ZeroVector, FRotator::ZeroRotator);
			Potion->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PotionSocket"));
			ABAnim->PlayDrinkPotionMontage();
		}
	}
}

bool APlayerCharacter::SetCharacterRotationToAxis()
{
	/** 이동키 입력에 따른 값(1 or -1)을 2D벡터에 저장 */
	FVector2D mov;
	mov.X = GetInputAxisValue("MoveForward");
	mov.Y = GetInputAxisValue("MoveRight");

	/** 컨트롤러의 Rotation 값을 가져온다 */
	const FRotator Rotation = GetController()->GetControlRotation();

	/** 새로 바뀔 캐릭터의 Rotation값 */
	FRotator YawRotation;

	if (mov.X == 0 && mov.Y == 0)
	{
		return false;
	}
	else if (mov.X == 0)
	{
		YawRotation = FRotator(0.f, Rotation.Yaw + (90.0f*mov.Y), 0.0f);
	}
	else if (mov.Y == 0)
	{
		YawRotation = FRotator(0.f, Rotation.Yaw - (90.0f*(mov.X - 1)), 0.0f);
	}
	else
	{
		YawRotation = FRotator(0.f, Rotation.Yaw + (((45 - (mov.X * 45)) + 45)*mov.Y), 0.0f);
	}

	SetActorRotation(YawRotation);

	return true;
}

void APlayerCharacter::EquipWeapon(TSubclassOf<class AWeapon> NewWeapon)
{
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon, FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetUnEquipPosion());
		CurrentWeapon->SetInstigator(GetController());
		CurrentWeapon->SetWeaponOwner(this);
		UE_LOG(LogTemp, Error, TEXT("Damage?? : %d"), GetCharacterStat()->GetAttack());
	}
}

void APlayerCharacter::EquipShield(TSubclassOf<class AShield> NewShield)
{
	CurrentShield = GetWorld()->SpawnActor<AShield>(NewShield, FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurrentShield != nullptr)
	{
		CurrentShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentShield->GetUnEquipPosion());
	}
}

void APlayerCharacter::UnEquipWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void APlayerCharacter::UnEquipShield()
{
	if (CurrentShield != nullptr)
	{
		CurrentShield->Destroy();
		CurrentShield = nullptr;
	}
}

void APlayerCharacter::TargetingLockOn()
{
	if (CombatMode == ECombatMode::NonCombat)
	{
		return;
	}

	float CloseDistance = 2000;

	if (TargetingEnemys.Num() > 0 && !bTargeting)
	{
		for (auto& Enemy : TargetingEnemys)
		{
			FRotator BetweenAngle = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation());
			FRotator EnemyLocation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenAngle);
			bool bDamageZone = (UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, -180.0f, -90.0f) || UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, 90.0f, 180.0f));

			if (!bDamageZone)
			{
				if (CloseDistance > GetDistanceTo(Enemy))
				{
					CloseDistance = GetDistanceTo(Enemy);
					TargetingTarget = Enemy;
				}
			}
		}
		if (TargetingTarget)
		{
			bTargeting = true;
			TargetingTarget->ShowTargetPoint(true);
			SetTargetingView();
		}
	}
	else if (bTargeting)
	{
		TargetingTarget->ShowTargetPoint(false);
		TargetingTarget = nullptr;

		bTargeting = false;

		SetPointOfView();
	}
}

void APlayerCharacter::AddNewItem()
{
	Inventory->AddItem(TEXT("철검"));
	Inventory->AddItem(TEXT("철대검"));
	Inventory->AddItem(TEXT("테스트방패"));
}

void APlayerCharacter::PlayBlocking()
{
	bPressedRMouse = true;
	if (CurrentBehavior != ECharacterBehavior::Idle && CurrentBehavior != ECharacterBehavior::Move)
	{
		return;
	}

	if (CombatMode == ECombatMode::Combat)
	{
		CurrentBehavior = ECharacterBehavior::Block;
		if (IsValid(CurrentShield))
		{
			ABAnim->PlayBlockingMontage();
		}
		else
		{
			ABAnim->PlayWeaponBlockingMontage(CurrentWeapon->GetWeaponType());
		}
	}
}

void APlayerCharacter::StopBlocking()
{
	bPressedRMouse = false;
	if (CurrentBehavior == ECharacterBehavior::Block)
	{
		CurrentBehavior = ECharacterBehavior::Idle;
		if (IsValid(CurrentShield))
		{
			ABAnim->StopBlockingMontage();
		}
		else
		{
			ABAnim->StopWeaponBlockingMontage(CurrentWeapon->GetWeaponType());
		}
	}
}

void APlayerCharacter::PlayInteraction()
{
	if (CombatMode == ECombatMode::Combat || CurrentBehavior != ECharacterBehavior::Idle)
	{
		return;
	}

	FRotator BetweenAngle;
	FRotator TargetLocation;
	bool bInteractionZone;

	if (InteractionTarget)
	{	
		switch (InteractionTarget->GetInteractionDetails())
		{
		case EInteractionDetail::None:
			InteractionTarget->BeginInteraction();
			break;
		case EInteractionDetail::BossDoor:
			BetweenAngle = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InteractionTarget->GetRootComponent()->GetRelativeLocation());
			TargetLocation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenAngle);
			bInteractionZone = (UKismetMathLibrary::InRange_FloatFloat(TargetLocation.Yaw, -180.0f, -20.0f) || UKismetMathLibrary::InRange_FloatFloat(TargetLocation.Yaw, 20.0f, 180.0f));
			if (!bInteractionZone)
			{
				CurrentBehavior = ECharacterBehavior::Interaction;
				ABAnim->PlayOpenBossDoorMontage();
				InteractionTarget->BeginInteraction();
			}
			break;
		case EInteractionDetail::Portal:
			Cast<AABPlayerController>(GetController())->VisiblePortalWidget();
			break;
		default:
			break;
		}
	}
}

void APlayerCharacter::TargetRangeOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* DetectedEnemy = Cast<AEnemy>(OtherActor);
		if (DetectedEnemy)
		{
			TargetingEnemys.Emplace(DetectedEnemy);
		}
	}
}

void APlayerCharacter::TargetRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AEnemy* DetectedEnemy = Cast<AEnemy>(OtherActor);
		if (DetectedEnemy)
		{
			TargetingEnemys.Remove(DetectedEnemy);
			if (TargetingTarget == DetectedEnemy)
			{
				TargetingLockOn();
			}
		}
	}
}

void APlayerCharacter::TestCode()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Shelter"));
}

void APlayerCharacter::CC_Stun(float PlayRate)
{
	CurrentBehavior = ECharacterBehavior::Stun;
	ABAnim->PlayCCMontages(PlayRate);
}

void APlayerCharacter::SwitchLevel(FName NewLevelName)
{

}

void APlayerCharacter::ShowPauseMenu()
{
	Cast<AABPlayerController>(GetController())->VisiblePauseMenu();
}

void APlayerCharacter::TestLevelUp()
{
	CharacterStat->LevelUp();
}

void APlayerCharacter::TestMoneyUp()
{
	Inventory->IncMoney(1000);
}