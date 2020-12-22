// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetPointWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TARGETPOINT"));
	TargetPointWidget->SetupAttachment(GetMesh());
	TargetPointWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TargetPointWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> TargetPoint_Widget(TEXT("/Game/HUD/EnemyHUD/BP_TargetingPoint.BP_TargetingPoint_C"));
	if (TargetPoint_Widget.Succeeded())
	{
		TargetPointWidget->SetWidgetClass(TargetPoint_Widget.Class);
		TargetPointWidget->SetDrawSize(FVector2D(10.0f, 10.0f));
		TargetPointWidget->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

