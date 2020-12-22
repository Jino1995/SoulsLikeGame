// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

UENUM(BlueprintType)
enum class EInteractionDetail :uint8
{
	None,
	BossDoor,
	Portal
};

UCLASS()
class GAMEPROJECT_API AInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginInteraction() {};
	virtual void EndInteraction() {};

	FORCEINLINE EInteractionDetail GetInteractionDetails() { return Details; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail", Meta = (AllowPrivateAccess = true))
	EInteractionDetail Details;
};
