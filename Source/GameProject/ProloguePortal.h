// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProloguePortal.generated.h"

UCLASS()
class GAMEPROJECT_API AProloguePortal : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProloguePortal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = StaticMesh, Meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
	class UBoxComponent* TransitionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FName TransitionLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FVector PortLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FRotator PortRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
