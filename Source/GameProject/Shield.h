// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class GAMEPROJECT_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE FName GetEquipPosion() { return EquipPosition; }
	FORCEINLINE FName GetUnEquipPosion() { return UnEquipPosition; }
	FORCEINLINE UParticleSystem* GetBlockParticle() { return BlockParticle; }
	FORCEINLINE FVector GetBlockSocket() { return Mesh->GetSocketLocation("BlockSocket"); }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* BlockParticle;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", Meta = (AllowPrivateAccess = true))
	FName EquipPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", Meta = (AllowPrivateAccess = true))
	FName UnEquipPosition;
};
