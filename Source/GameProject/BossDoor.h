// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "InteractionActor.h"
#include "GameFramework/Actor.h"
#include "BossDoor.generated.h"

UCLASS()
class GAMEPROJECT_API ABossDoor : public AInteractionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InteractionZoneOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void InteractionZoneOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void ClosedZoneOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ClosedZoneOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void BeginInteraction() override;
	virtual void EndInteraction() override;

private:
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* OpenTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset", Meta = (AllowPrivateAccess = true))
	class USoundBase* OpenSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh, Meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* InteractionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UBoxComponent* ClosedCollision;

	bool bOpening;
	bool bOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail", Meta = (AllowPrivateAccess = true))
	FRotator DoorCloseRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detail", Meta = (AllowPrivateAccess = true))
	FRotator DoorOpenRotation;
};
