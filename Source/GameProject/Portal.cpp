// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "PlayerCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTROOT"));
	SetRootComponent(DefaultRootComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BODY_SM(TEXT("/Game/PaulosCreations/DemonicAltar/Meshes/SM_DemonicAltar.SM_DemonicAltar"));
	if (BODY_SM.Succeeded())
	{
		BodyMesh->SetStaticMesh(BODY_SM.Object);
	}
	BodyMesh->SetupAttachment(RootComponent);

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	TransitionVolume->SetupAttachment(RootComponent);

	InteractionTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OPENTEXT"));
	InteractionTextWidget->SetupAttachment(RootComponent);
	InteractionTextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	InteractionTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/HUD/InteractionText/BP_InteractionText.BP_InteractionText_C"));
	if (UI_HUD.Succeeded())
	{
		InteractionTextWidget->SetWidgetClass(UI_HUD.Class);
		InteractionTextWidget->SetDrawSize(FVector2D(150.0f, 100.0f));
		InteractionTextWidget->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	TransitionVolume->OnComponentEndOverlap.AddDynamic(this, &APortal::OnOverlapEnd);
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			Player->SetInteractionTarget(this);
			InteractionTextWidget->SetVisibility(true);
		}
	}
}

void APortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			Player->SetInteractionTarget(nullptr);
			InteractionTextWidget->SetVisibility(false);
		}
	}
}