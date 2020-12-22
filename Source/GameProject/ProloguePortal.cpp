// Fill out your copyright notice in the Description page of Project Settings.


#include "ProloguePortal.h"
#include "PlayerCharacter.h"
#include "ABPlayerController.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
AProloguePortal::AProloguePortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTROOT"));
	SetRootComponent(DefaultRootComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BODY_SM(TEXT("/Game/Portal/portal_subs_Box001.portal_subs_Box001"));
	if (BODY_SM.Succeeded())
	{
		BodyMesh->SetStaticMesh(BODY_SM.Object);
	}
	BodyMesh->SetupAttachment(RootComponent);

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PORTAL"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PORTAL_SM(TEXT("/Game/Portal/portal_subs_Plane001.portal_subs_Plane001"));
	if (PORTAL_SM.Succeeded())
	{
		PortalMesh->SetStaticMesh(PORTAL_SM.Object);
	}
	PortalMesh->SetupAttachment(RootComponent);

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	TransitionVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProloguePortal::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &AProloguePortal::OnOverlapBegin);
}

// Called every frame
void AProloguePortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProloguePortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			Cast<AABPlayerController>(Player->GetController())->ChangeLevel(TransitionLevelName, PortLocation, PortRotation);
		}
	}
}