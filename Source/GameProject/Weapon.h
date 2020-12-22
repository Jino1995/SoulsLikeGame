// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "CharacterStatComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
	OneHandWeapon,
	GreatSword,
	TwoHandWeapon,
	Spear,
	Katana
};

UCLASS()
class GAMEPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FTimerHandle TraceTimer;

	void ActivateCollision();
	void DeActivateCollision();

	UFUNCTION()
	void EnableAttackBoxTrace();

	bool bTraceEnabled;

public:
	FORCEINLINE void SetWeaponOwner(class APlayerCharacter* OwnerCharacter) { WeaponOwner = OwnerCharacter; }
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
	FORCEINLINE FName GetEquipPosion() { return EquipPosition; }
	FORCEINLINE FName GetUnEquipPosion() { return UnEquipPosition; }
	FORCEINLINE EWeaponTypes GetWeaponType() { return WeaponType; }
	FORCEINLINE float GetAttackStamina() { return AttackStamina; }
	FORCEINLINE float GetStrongAttackStamina() { return StrongAttackStamina; }
	FORCEINLINE USoundBase* GetEquippingSound() { return EquippingSound; }
	FORCEINLINE USoundBase* GetSwingSound() { return SwingSound; }
	FORCEINLINE UParticleSystem* GetBlockParticle() { return BlockParticle; }
	FORCEINLINE FVector GetBlockSocket() { return Mesh->GetSocketLocation("BlockSocket"); }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* BlockParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class USoundBase* EquippingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class USoundBase* SwingSound;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Types", Meta = (AllowPrivateAccess = true))
	EWeaponTypes WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", Meta = (AllowPrivateAccess = true))
	FName EquipPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", Meta = (AllowPrivateAccess = true))
	FName UnEquipPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float StrongAttackStamina;

	TSet<class AEnemy*> DamagedEnemy;

	UPROPERTY()
	class APlayerCharacter* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	AController* WeaponInstigator;
};
