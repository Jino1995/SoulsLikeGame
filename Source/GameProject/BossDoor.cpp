// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDoor.h"
#include "PlayerCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABossDoor::ABossDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTROOT"));
	SetRootComponent(DefaultRootComponent);
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("L_DOOR"));
	LeftDoor->SetupAttachment(RootComponent);
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("R_DOOR"));
	RightDoor->SetupAttachment(RootComponent);

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("INTERACTIONZONE"));
	InteractionCollision->SetupAttachment(RootComponent);

	ClosedCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CLOSEDZONE"));
	ClosedCollision->SetupAttachment(RootComponent);

	DoorCloseRotation = LeftDoor->GetRelativeRotation();

	OpenTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OPENTEXT"));
	OpenTextWidget->SetupAttachment(RootComponent);
	OpenTextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	OpenTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/HUD/InteractionText/BP_OpenDoorText.BP_OpenDoorText_C"));
	if (UI_HUD.Succeeded())
	{
		OpenTextWidget->SetWidgetClass(UI_HUD.Class);
		OpenTextWidget->SetDrawSize(FVector2D(150.0f, 100.0f));
		OpenTextWidget->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void ABossDoor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossDoor::InteractionZoneOnOverlapBegin);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ABossDoor::InteractionZoneOnOverlapEnd);

	ClosedCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossDoor::ClosedZoneOnOverlapBegin);
	ClosedCollision->OnComponentEndOverlap.AddDynamic(this, &ABossDoor::ClosedZoneOnOverlapEnd);
}

// Called every frame
void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening)
	{
		FRotator InterpRotation = FMath::RInterpTo(LeftDoor->GetRelativeRotation(), DoorOpenRotation, DeltaTime, 0.5f);
		LeftDoor->SetRelativeRotation(InterpRotation);
		RightDoor->SetRelativeRotation(InterpRotation*-1);
	}
}

void ABossDoor::InteractionZoneOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			Player->SetInteractionTarget(this);
			OpenTextWidget->SetVisibility(true);
		}
	}
}

void ABossDoor::InteractionZoneOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			Player->SetInteractionTarget(nullptr);
		}
	}
}

void ABossDoor::ClosedZoneOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			LeftDoor->SetRelativeRotation(DoorCloseRotation);
			RightDoor->SetRelativeRotation(DoorCloseRotation * -1);
		}
	}
}

void ABossDoor::ClosedZoneOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABossDoor::BeginInteraction()
{
	bOpening = true;
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
	OpenTextWidget->SetVisibility(false);
}

void ABossDoor::EndInteraction()
{
	bOpening = false;
	bOpen = true;
}