// Fill out your copyright notice in the Description page of Project Settings.


#include "DevilDoor.h"
#include "PlayerCharacter.h"

// Sets default values
ADevilDoor::ADevilDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTROOT"));
	SetRootComponent(DefaultRootComponent);
	
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));
	Door->SetupAttachment(RootComponent);

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("INTERACTIONZONE"));
	InteractionCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADevilDoor::BeginPlay()
{
	Super::BeginPlay();

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ADevilDoor::InteractionZoneOnOverlapBegin);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ADevilDoor::InteractionZoneOnOverlapEnd);
}

// Called every frame
void ADevilDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening)
	{
		FRotator InterpRotation = FMath::RInterpTo(Door->GetRelativeRotation(), DoorOpenRotation, DeltaTime, 0.5f);
		Door->SetRelativeRotation(InterpRotation);
	}
}

void ADevilDoor::InteractionZoneOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			if (!Player->IsBossBattle())
			{
				Player->SetInteractionTarget(this);
			}
		}
	}
}

void ADevilDoor::InteractionZoneOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			if (!Player->IsBossBattle())
			{
				Player->SetInteractionTarget(nullptr);
			}
		}
	}
}

void ADevilDoor::BeginInteraction()
{
	bOpening = true;
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}

void ADevilDoor::EndInteraction()
{
	bOpening = false;
	bOpen = true;
}
