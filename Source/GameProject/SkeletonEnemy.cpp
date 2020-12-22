// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"
#include "SkeletonEnemyAnimInstance.h"
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

ASkeletonEnemy::ASkeletonEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PAWNSENSING"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Game/SekeltonLight/Meshes/SkeletonLight_rig.SkeletonLight_rig"));
	if (SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mesh.Object);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_Anim(TEXT("/Game/SekeltonLight/Blueprint/BP_SkeletonEnemy_Anim.BP_SkeletonEnemy_Anim_C"));
	if (BP_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_Anim.Class);
	}

	CombatRange = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
	CombatRange->SetupAttachment(RootComponent);
	CombatRange->InitSphereRadius(75.0f);

	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ATTACKCOLLISION"));
	
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurrentPatrolNum = 0;
	bPatrol = false;
	bTargetOnAttackRange = false;
	bDead = false;
	bRotate = false;

	CurrentEMS = EEnemyMovementStatus::Idle;

	CombatTarget = nullptr;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/HUD/EnemyHUD/BP_NormalEnemyHPBar.BP_NormalEnemyHPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 100.0f));
		HPBarWidget->SetVisibility(false);
	}

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
void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.0f, 0.0f);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	CombatRange->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonEnemy::CombatRangeOnOverlapBegin);
	CombatRange->OnComponentEndOverlap.AddDynamic(this, &ASkeletonEnemy::CombatRangeOnOverlapEnd);
	
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonEnemy::AttackCollisionOnOverlapBegin);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &ASkeletonEnemy::AttackCollisionOnOverlapEnd);

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	if (bPatrol)
	{
		CurrentEMS = EEnemyMovementStatus::Patrol;
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ASkeletonEnemy::PlayPatrol, 1, true);
	}

	auto CWidget = Cast<UNormalEnemyHPBar>(HPBarWidget->GetUserWidgetObject());
	if (CWidget != nullptr)
	{
		CWidget->BindCharacterStat(this);
	}

}

void ASkeletonEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<USkeletonEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	Sensing->OnSeePawn.AddDynamic(this, &ASkeletonEnemy::OnSeePawn);

	AIController = Cast<AAIController>(GetController());

	AttackCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("AttackSocket"));

	AnimInstance->OnDamagedEnd.AddLambda([this]() -> void {
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnAttackEnd.AddLambda([this]() -> void {
		bAttacking = false;
		CurrentEMS = EEnemyMovementStatus::Idle;
	});

	AnimInstance->OnAttackStart.AddLambda([this]() -> void {
		CurrentEMS = EEnemyMovementStatus::Attacking;
	});

	AnimInstance->OnActivateAttack.AddLambda([this]() -> void {
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	});

	AnimInstance->OnDeActivateAttack.AddLambda([this]() -> void {
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	});
}

// Called every frame
void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CombatTarget != nullptr && CurrentEMS != EEnemyMovementStatus::Damaged && CurrentEMS != EEnemyMovementStatus::Attacking)
	{
		if (bTargetOnCombatRange)
		{
			CheckTargetOnAttackRange();
		}

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
		FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotationYaw, DeltaTime, 5.0f);

		SetActorRotation(InterpRotation);
	}
}

// Called to bind functionality to input
void ASkeletonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASkeletonEnemy::OnSeePawn(APawn *OtherPawn)
{
	if (bDead)
	{
		Sensing->bSeePawns = false;
		return;
	}

	if (bPatrol)
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
	}

	Sensing->bSeePawns = false;

	CombatTarget = Cast<APlayerCharacter>(OtherPawn);
	GetWorldTimerManager().SetTimer(MoveToTargetTimer, this, &ASkeletonEnemy::MoveToTarget, 0.5, true);


	GetWorldTimerManager().SetTimer(AttackTimer, this, &ASkeletonEnemy::Attack, 0.5, true);

	GetCharacterMovement()->MaxWalkSpeed = MoveToTargetSpeed;
}

void ASkeletonEnemy::MoveToTarget()
{
	if (CurrentEMS == EEnemyMovementStatus::Damaged || CurrentEMS == EEnemyMovementStatus::Attacking)
	{
		AIController->StopMovement();
		return;
	}

	CurrentEMS = EEnemyMovementStatus::MoveToTarget;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetAcceptanceRadius(0.0f);
	MoveRequest.SetGoalActor(CombatTarget);
	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest, &NavPath);
}

void ASkeletonEnemy::PlayPatrol()
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetAcceptanceRadius(0.0f);
	FNavPathSharedPtr NavPath;

	if (PatrolResult)
	{
		if (PatrolResult.Code == EPathFollowingRequestResult::RequestSuccessful)
		{
			PatrolResult = AIController->MoveTo(MoveRequest, &NavPath);
		}
		else if (PatrolResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			CurrentPatrolNum = (CurrentPatrolNum + 1) % PatrolPoint.Num();
			MoveRequest.SetGoalLocation(PatrolPoint[CurrentPatrolNum]);
			PatrolResult = AIController->MoveTo(MoveRequest, &NavPath);
		}
	}
	else
	{
		MoveRequest.SetGoalLocation(PatrolPoint[CurrentPatrolNum]);
		PatrolResult = AIController->MoveTo(MoveRequest, &NavPath);
	}
}

void ASkeletonEnemy::CombatRangeOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* OverlapPawn = Cast<APlayerCharacter>(OtherActor);
		if (OverlapPawn)
		{
			bTargetOnCombatRange = true;
		}
	}
}

void ASkeletonEnemy::CombatRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* OverlapPawn = Cast<APlayerCharacter>(OtherActor);
		if (OverlapPawn)
		{
			bTargetOnCombatRange = false;
			bTargetOnAttackRange = false;
		}
	}
}

void ASkeletonEnemy::AttackCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Pawn = Cast<APlayerCharacter>(OtherActor);
		if (Pawn)
		{
			AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			float AttackResult = UGameplayStatics::ApplyDamage(Pawn, EnemyDamage, AIController, this, DamageTypeClass);
			if (AttackResult == -1)
			{
				AnimInstance->JumpToBlockedAttackSection();
			}
		}
	}
}

void ASkeletonEnemy::AttackCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

float ASkeletonEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	AIController->StopMovement();

	HPBarWidget->SetVisibility(true);
	
	if (!IsValid(CombatTarget))
	{
		OnSeePawn(Cast<APlayerCharacter>(DamageCauser));
	}
	
	if (bAttacking)
	{
		bAttacking = false;
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (DecHealth(DamageAmount))
	{
		CurrentEMS = EEnemyMovementStatus::Damaged;
		AnimInstance->PlayTakeDamageMontage();
	}
	else
	{
		Dead();
	}

	return DamageAmount;
}

bool ASkeletonEnemy::DecHealth(float DecAmount)
{
	auto DWidget = Cast<UHPFloatingText>(FloatingTextManager->GetUserWidgetObject());
	if (DWidget != nullptr)
	{
		DWidget->CreateDamageText(DecAmount);
	}

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

void ASkeletonEnemy::IncHealth(float incAmount)
{

}

void ASkeletonEnemy::Dead()
{
	bDead = true;
	GetWorldTimerManager().ClearTimer(MoveToTargetTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);

	AIController->StopMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HPBarWidget->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = false;

	CurrentEMS = EEnemyMovementStatus::Dead;

	ItemDrop();
}

void ASkeletonEnemy::CheckTargetOnAttackRange()
{
	FRotator BetweenAngle = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CombatTarget->GetActorLocation());
	FRotator EnemyLocation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), BetweenAngle);
	bool bDamageZone = (UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, -180.0f, -90.0f) || UKismetMathLibrary::InRange_FloatFloat(EnemyLocation.Yaw, 90.0f, 180.0f));
	if (bDamageZone)
	{
		bTargetOnAttackRange = false;
	}
	else
	{
		bTargetOnAttackRange = true;
	}
}

void ASkeletonEnemy::Attack()
{
	if (!bTargetOnAttackRange || CurrentEMS == EEnemyMovementStatus::Damaged || CurrentEMS == EEnemyMovementStatus::Attacking || bAttacking)
	{
		return;
	}
	bAttacking = true;
	AnimInstance->PlayAttackMontage();
}

void ASkeletonEnemy::ItemDrop()
{
	if (DropItems.Num() == 0)
	{
		return;
	}

	for (auto Item : DropItems)
	{
		if (Item.Value >= (rand() % 100))
		{
			CombatTarget->GetInventory()->AddItem(Item.Key);
		}
	}
}