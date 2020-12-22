// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include "CharacterStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));

	SetRootComponent(Mesh);

	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTraceEnabled)
	{
		EnableAttackBoxTrace();
	}
}

void AWeapon::ActivateCollision()
{
	UE_LOG(LogTemp, Error, TEXT("%d"), WeaponOwner->GetCharacterStat()->GetAttack());
	bTraceEnabled = true;
}

void AWeapon::DeActivateCollision()
{
	DamagedEnemy.Empty();
	bTraceEnabled = false;
}

void AWeapon::EnableAttackBoxTrace()
{	
	TArray<FHitResult> HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(EObjectTypeQuery::ObjectTypeQuery3));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(WeaponOwner);

	bool bResult = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Mesh->GetSocketLocation(FName("AttackStart")), Mesh->GetSocketLocation(FName("AttackEnd")), FVector(5.f, 5.f, 5.f), FRotator(0.f, 0.f, 0.f), ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);
	
	if (bResult)
	{
		for(auto Result : HitResult)
		{
			AEnemy* Enemy = Cast<AEnemy>(Result.GetActor());
			if (Enemy && DamageTypeClass)
			{
				if (!DamagedEnemy.Contains(Enemy))
				{
					DamagedEnemy.Emplace(Enemy);
					if (WeaponOwner->IsStrongAttack())
					{
						float DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, WeaponOwner->GetCharacterStat()->GetAttack() * 1.5, Result.TraceStart, Result, WeaponInstigator, WeaponOwner, DamageTypeClass);
						if (DamageResult > 0)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->GetHitParticles(), Result.ImpactPoint, FRotator(0.0f), false);
						}
					}
					else
					{
						float DamageResult = UGameplayStatics::ApplyPointDamage(Enemy, WeaponOwner->GetCharacterStat()->GetAttack(), Result.TraceStart, Result, WeaponInstigator, WeaponOwner, DamageTypeClass);
						if (DamageResult > 0)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->GetHitParticles(), Result.ImpactPoint, FRotator(0.f), false);
						}
					}
				}
			}
		}
	}
}