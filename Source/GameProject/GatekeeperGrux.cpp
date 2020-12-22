// Fill out your copyright notice in the Description page of Project Settings.


#include "GatekeeperGrux.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "NormalEnemyHPBar.h"
#include "Components/WidgetComponent.h"
#include "HPFloatingText.h"
#include "InventoryComponent.h"
#include "TimerManager.h"
#include "GatekeeperGruxAnimInstance.h"
#include "ABPlayerController.h"
#include "BossHPBar.h"
#include "Sound/SoundCue.h"
#include "InventoryComponent.h"

AGatekeeperGrux::AGatekeeperGrux()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PAWNSENSING"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_Anim(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/BP_GateKeeperGruxAnim.BP_GateKeeperGruxAnim_C"));
	if (BP_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_Anim.Class);
	}

	JumpAttackCollision1 = CreateDefaultSubobject<USphereComponent>(TEXT("JUMPCOLLISION1"));
	JumpAttackCollision1->SetupAttachment(RootComponent);
	JumpAttackCollision2 = CreateDefaultSubobject<USphereComponent>(TEXT("JUMPCOLLISION2"));
	JumpAttackCollision2->SetupAttachment(RootComponent);

	RoarAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ROARATTACKCOLLISION"));
	RoarAttackCollision->SetupAttachment(RootComponent);

	TwinRoar_RoarCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TWINROARCOLLISION"));
	TwinRoar_RoarCollision->SetupAttachment(RootComponent);

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	bEnableJumpAttack = true;
	bEnableRoarAttack = true;
	bEnableTwinAttack = true;
	bEnableTwinRoarAttack = true;
}

void AGatekeeperGrux::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	JumpAttackCollision1->OnComponentBeginOverlap.AddDynamic(this, &AGatekeeperGrux::JumpAttackCollision1OnOverlapBegin);
	JumpAttackCollision1->OnComponentEndOverlap.AddDynamic(this, &AGatekeeperGrux::JumpAttackCollision1OnOverlapEnd);
	JumpAttackCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	JumpAttackCollision2->OnComponentBeginOverlap.AddDynamic(this, &AGatekeeperGrux::JumpAttackCollision2OnOverlapBegin);
	JumpAttackCollision2->OnComponentEndOverlap.AddDynamic(this, &AGatekeeperGrux::JumpAttackCollision2OnOverlapEnd);
	JumpAttackCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RoarAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AGatekeeperGrux::RoarAttackCollisionOnOverlapBegin);
	RoarAttackCollision->OnComponentEndOverlap.AddDynamic(this, &AGatekeeperGrux::RoarAttackCollisionOnOverlapEnd);
	RoarAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TwinRoar_RoarCollision->OnComponentBeginOverlap.AddDynamic(this, &AGatekeeperGrux::TwinRoarAttackCollisionOnOverlapBegin);
	TwinRoar_RoarCollision->OnComponentEndOverlap.AddDynamic(this, &AGatekeeperGrux::TwinRoarAttackCollisionOnOverlapEnd);
	TwinRoar_RoarCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGatekeeperGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget && CurrentEMS != EEnemyMovementStatus::Attacking &&  CurrentEMS != EEnemyMovementStatus::Dead)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotationYaw, DeltaTime, 5.0f);

		SetActorRotation(InterpRotation);

		if (DistanceToEnemy > 500)
		{
			MoveToTarget();
		}
		else
		{
			CurrentEMS = EEnemyMovementStatus::Strafe;
			AIController->StopMovement();
		
			if (CurrentEMS == EEnemyMovementStatus::Strafe)
			{
				switch (StrafeDirection)
				{
				case EStrafeDirection::Right:
					AddMovementInput(GetActorRightVector(), 1);
					break;
				case EStrafeDirection::Left:
					AddMovementInput(GetActorRightVector(), -1);
					break;
				}
			}	
		}
	}
}

void AGatekeeperGrux::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Sensing->OnSeePawn.AddDynamic(this, &AGatekeeperGrux::OnSeePawn);

	AIController = Cast<AAIController>(GetController());

	AnimInstance = Cast<UGatekeeperGruxAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnAttackEnd.AddLambda([this]() -> void {
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnLeftWeaponOn.AddLambda([this]() -> void {
		GetWorldTimerManager().SetTimer(LeftWeaponTraceTimer, this, &AGatekeeperGrux::EnableLeftWeaponTrace, 0.01f, true);
	});

	AnimInstance->OnLeftWeaponOff.AddLambda([this]() -> void {
		DisableLeftWeaponTrace();
	});

	AnimInstance->OnRightWeaponOn.AddLambda([this]() -> void {
		GetWorldTimerManager().SetTimer(RightWeaponTraceTimer, this, &AGatekeeperGrux::EnableRightWeaponTrace, 0.01f, true);
	});

	AnimInstance->OnRightWeaponOff.AddLambda([this]() -> void {
		DisableRightWeaponTrace();
	});

	AnimInstance->OnJumpAttackEnd.AddLambda([this]() -> void {
		float JumpAttackCoolTime = FMath::FRandRange(MinJumpAttackCoolTime, MaxJumpAttackCoolTime);
		GetWorldTimerManager().SetTimer(JumpAttackCoolTimeTimer, this, &AGatekeeperGrux::EnableJumpAttack, JumpAttackCoolTime, false);
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnRoarEnd.AddLambda([this]() -> void {
		float RoarAttackCoolTime = FMath::FRandRange(MinRoarAttackCoolTime, MaxRoarAttackCoolTime);
		GetWorldTimerManager().SetTimer(RoarAttackCoolTimeTimer, this, &AGatekeeperGrux::EnableRoarAttack, RoarAttackCoolTime, false);
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnTwinAttackEnd.AddLambda([this]() -> void {
		float TwinAttackCoolTime = FMath::FRandRange(MinTwinAttackCoolTime, MaxTwinAttackCoolTime);
		GetWorldTimerManager().SetTimer(TwinAttackCoolTimeTimer, this, &AGatekeeperGrux::EnableTwinAttack, TwinAttackCoolTime, false);
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnJumpAttack1HitCheck.AddLambda([this]() -> void {
		JumpAttackCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	});
	AnimInstance->OnJumpAttack1End.AddLambda([this]() -> void {
		JumpAttackCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});

	AnimInstance->OnJumpAttack2HitCheck.AddLambda([this]() -> void {
		JumpAttackCollision2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	});
	AnimInstance->OnJumpAttack2End.AddLambda([this]() -> void {
		JumpAttackCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});

	AnimInstance->OnRoarAttackCheck.AddLambda([this]() -> void {
		RoarAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	});
	AnimInstance->OnDisableRoarAttack.AddLambda([this]() -> void {
		RoarAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});

	AnimInstance->OnEnableTwinRoar.AddLambda([this]() -> void {
		TwinRoar_RoarCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	});
	AnimInstance->OnDisableTwinRoar.AddLambda([this]() -> void {
		TwinRoar_RoarCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});
	AnimInstance->OnTwinRoarAttackEnd.AddLambda([this]() -> void {
		float TwinRoarAttackCoolTime = FMath::FRandRange(MinTwinRoarAttackCoolTime, MaxTwinRoarAttackCoolTime);
		GetWorldTimerManager().SetTimer(TwinRoarAttackCoolTimeTimer, this, &AGatekeeperGrux::EnableTwinRoarAttack, TwinRoarAttackCoolTime, false);
		CurrentEMS = EEnemyMovementStatus::Idle;
	});
}

float AGatekeeperGrux::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	UGameplayStatics::PlaySound2D(GetWorld(), HitSound, 1.0f, 1.0f, 0.f);
	
	if (DecHealth(DamageAmount))
	{
		BossHPBar->UpdateDamageText(DamageAmount);
	}
	else
	{
		BossHPBar->UpdateDamageText(DamageAmount);
		Dead();
	}
	return DamageAmount;
}

void AGatekeeperGrux::Dead()
{
	Music->Stop();
	PrimaryActorTick.SetTickFunctionEnable(false);
	CurrentEMS = EEnemyMovementStatus::Dead;
	CombatTarget->SetbBossBattle(false);

	GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
	GetWorldTimerManager().ClearTimer(StrafingTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	
	AIController->StopMovement();
	bDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cast<AABPlayerController>(CombatTarget->GetController())->RequestShowBossHPBarToHUD(this);

	CombatTarget->GetInventory()->AddItem(TEXT("그럭스의영혼"));
}

void AGatekeeperGrux::JumpAttackCollision1OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			JumpAttackCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			float AttackResult = Pawn->TakeDamage_CanNotBlock(JumpAttack1Damage);

		}
	}
}

void AGatekeeperGrux::JumpAttackCollision1OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AGatekeeperGrux::JumpAttackCollision2OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			JumpAttackCollision2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			float AttackResult = Pawn->TakeDamage_CanNotBlock(JumpAttack2Damage);
		}
	}
}

void AGatekeeperGrux::JumpAttackCollision2OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AGatekeeperGrux::RoarAttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			RoarAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Pawn->TakeDamage_CanNotBlock(RoarAttackDamage);
			Pawn->CC_Stun(RoarAttackStunRate);
		}
	}
}

void AGatekeeperGrux::RoarAttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AGatekeeperGrux::TwinRoarAttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			TwinRoar_RoarCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Pawn->TakeDamage_CanNotBlock(TwinRoarAttack_RoarDamage);
			Pawn->CC_Stun(TwinRoarAttack_RoarStunRate);
		}
	}
}

void AGatekeeperGrux::TwinRoarAttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AGatekeeperGrux::EnableLeftWeaponTrace()
{
	TArray<FHitResult> HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(EObjectTypeQuery::ObjectTypeQuery3));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bResult = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), GetMesh()->GetSocketLocation(FName("LeftWeaponStart")), GetMesh()->GetSocketLocation(FName("LeftWeaponEnd")), FVector(10.f, 10.f, 10.f), FRotator(0.f, 0.f, 0.f), ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	if (bResult)
	{
		for (auto Result : HitResult)
		{
			APlayerCharacter* Enemy = Cast<APlayerCharacter>(Result.GetActor());
			if (Enemy && DamageTypeClass)
			{
				if (!DamagedEnemy.Contains(Enemy))
				{
					DamagedEnemy.Emplace(Enemy);
					
					float DamageResult = 0.f;
					switch (WeaponAttackType)
					{
					case EWeaponAttackType::NormalAttack:
						ShieldBlockDamage = 30.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, NormalAttackDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						break;
					case EWeaponAttackType::TwinAttack:
						ShieldBlockDamage = 50.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, TwinAttackDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						break;
					case EWeaponAttackType::TwinRoarAttack:
						ShieldBlockDamage = 50.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, TwinRoarAttack_TwinDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						if (DamageResult > 0.f)
						{
							Enemy->CC_Stun(TwinRoarAttack_TwinStunRate);
						}
						break;
					default:
						break;
					}

					if (DamageResult > 0)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->GetHitParticles(), Result.ImpactPoint, FRotator(0.0f), false);
					}
				}
			}
		}
	}
}

void AGatekeeperGrux::DisableLeftWeaponTrace()
{
	GetWorldTimerManager().ClearTimer(LeftWeaponTraceTimer);
	DamagedEnemy.Empty();
}

void AGatekeeperGrux::EnableRightWeaponTrace()
{
	TArray<FHitResult> HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(EObjectTypeQuery::ObjectTypeQuery3));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bResult = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), GetMesh()->GetSocketLocation(FName("RightWeaponStart")), GetMesh()->GetSocketLocation(FName("RightWeaponEnd")), FVector(10.f, 10.f, 10.f), FRotator(0.f, 0.f, 0.f), ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	if (bResult)
	{
		for (auto Result : HitResult)
		{
			APlayerCharacter* Enemy = Cast<APlayerCharacter>(Result.GetActor());
			if (Enemy && DamageTypeClass)
			{
				if (!DamagedEnemy.Contains(Enemy))
				{
					DamagedEnemy.Emplace(Enemy);
					
					float DamageResult = 0.f;
					switch (WeaponAttackType)
					{
					case EWeaponAttackType::NormalAttack:
						ShieldBlockDamage = 30.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, NormalAttackDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						break;
					case EWeaponAttackType::TwinAttack:
						ShieldBlockDamage = 50.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, TwinAttackDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						break;
					case EWeaponAttackType::TwinRoarAttack:
						ShieldBlockDamage = 50.f;
						DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, TwinRoarAttack_TwinDamage, Result.TraceStart, Result, GetController(), this, DamageTypeClass);
						if (DamageResult > 0.f)
						{
							Enemy->CC_Stun(TwinRoarAttack_TwinStunRate);
						}
						break;
					default:
						break;
					}

					if (DamageResult > 0)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->GetHitParticles(), Result.ImpactPoint, FRotator(0.0f), false);
					}
				}
			}
		}
	}
}

void AGatekeeperGrux::DisableRightWeaponTrace()
{
	GetWorldTimerManager().ClearTimer(RightWeaponTraceTimer);
	DamagedEnemy.Empty();
}

void AGatekeeperGrux::EnableJumpAttack()
{
	bEnableJumpAttack = true;
}

void AGatekeeperGrux::EnableRoarAttack()
{
	bEnableRoarAttack = true;
}

void AGatekeeperGrux::EnableTwinAttack()
{
	bEnableTwinAttack = true;
}

void AGatekeeperGrux::EnableTwinRoarAttack()
{
	bEnableTwinRoarAttack = true;
}

void AGatekeeperGrux::OnSeePawn(APawn *OtherPawn)
{
	Music = UGameplayStatics::SpawnSound2D(GetWorld(), ThemeMusic, 1.f, 1.f, 0.f);
	Sensing->bSeePawns = false;
	CombatTarget = Cast<APlayerCharacter>(OtherPawn);
	Cast<AABPlayerController>(CombatTarget->GetController())->RequestShowBossHPBarToHUD(this);
	CombatTarget->SetbBossBattle(true);
	MoveToTarget();
	GetWorldTimerManager().SetTimer(CheckDistanceTimer, this, &AGatekeeperGrux::CheckDistanceToEnemy, 0.5f, true);
	GetWorldTimerManager().SetTimer(StrafingTimer, this, &AGatekeeperGrux::PlayStrafing, 3.0f, true);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AGatekeeperGrux::AttackSelect, 1.f, false);
}

void AGatekeeperGrux::PlayStrafing()
{	
	int32 Direction = rand() % 3;

	switch (Direction)
	{
	case 0:
		StrafeDirection = EStrafeDirection::Right;
		StrafeDirectionValue = 1;
		break;
	case 1:
		StrafeDirection = EStrafeDirection::Left;
		StrafeDirectionValue = -1;
		break;
	}
}

void AGatekeeperGrux::CheckDistanceToEnemy()
{
	DistanceToEnemy = GetDistanceTo(CombatTarget);
}

void AGatekeeperGrux::MoveToTarget()
{
	StrafeDirectionValue = 0;
	CurrentEMS = EEnemyMovementStatus::MoveToTarget;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetAcceptanceRadius(0.0f);
	MoveRequest.SetGoalActor(CombatTarget);
	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest, &NavPath);
}

void AGatekeeperGrux::AttackSelect()
{
	if (CurrentEMS != EEnemyMovementStatus::Attacking)
	{
		if (DistanceToEnemy < 300 && bEnableJumpAttack)
		{
			CurrentEMS = EEnemyMovementStatus::Attacking;
			AIController->StopMovement();
			GetWorldTimerManager().SetTimer(DelayTimer, this, &AGatekeeperGrux::PlayJumpAttack, 0.1f, false);
		}
		else if (DistanceToEnemy < 600)
		{
			CurrentEMS = EEnemyMovementStatus::Attacking;
			AIController->StopMovement();
			int32 SelectNum = rand() % 4;
			switch (SelectNum)
			{
			case 0:
				WeaponAttackType = EWeaponAttackType::NormalAttack;
				PlayNormalAttack();
				break;
			case 1:
				if (bEnableRoarAttack)
				{
					PlayRoarAttack();
				}
				else
				{
					PlayNormalAttack();
				}
				break;
			case 2:
				if (bEnableTwinAttack)
				{
					WeaponAttackType = EWeaponAttackType::TwinAttack;
					PlayTwinAttack();
				}
				else
				{
					WeaponAttackType = EWeaponAttackType::NormalAttack;
					PlayNormalAttack();
				}
				break;
			case 3:
				if (bEnableTwinRoarAttack)
				{
					WeaponAttackType = EWeaponAttackType::TwinRoarAttack;
					PlayTwinRoarAttack();
				}
				else
				{
					WeaponAttackType = EWeaponAttackType::NormalAttack;
					PlayNormalAttack();
				}
				break;
			default:
				break;
			}
		}
	}

	float AttackCheckTime = FMath::FRandRange(MinAttackCheckTime, MaxAttackCheckTime);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AGatekeeperGrux::AttackSelect, AttackCheckTime, false);
}

void AGatekeeperGrux::PlayNormalAttack()
{
	AnimInstance->PlayAttackMontage();
}

void AGatekeeperGrux::PlayJumpAttack()
{
	bEnableJumpAttack = false;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

void AGatekeeperGrux::PlayRoarAttack()
{
	bEnableRoarAttack = false;
	AnimInstance->PlayRoarAttackMontage();
}

void AGatekeeperGrux::PlayTwinAttack()
{
	bEnableTwinAttack = false;
	AnimInstance->PlayTwinAttackMontage();
}

void AGatekeeperGrux::PlayTwinRoarAttack()
{
	bEnableTwinRoarAttack = false;
	AnimInstance->PlayTwinRoarAttackMontage();
}

bool AGatekeeperGrux::DecHealth(float DecAmount)
{
	CurrentHealth -= DecAmount;
	if (CurrentHealth > 0)
	{
		OnHPChanged.Broadcast();
		return true;
	}
	else
	{
		CurrentHealth = 0;
		OnHPChanged.Broadcast();
		return false;
	}
}