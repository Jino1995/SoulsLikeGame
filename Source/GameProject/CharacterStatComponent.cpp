// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "PlayerCharacter.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	StaminaRate = 20.f;
	ConsumeStaminaRate = 30.f;

	NormalSpeed = 350.0f;
	WalkSpeed = 150.0f;
	SprintSpeed = 500.0f;
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	 //InitStat();
	// ...

}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float DeltaStamina = StaminaRate * DeltaTime;
	float DeltaConsumeStamina = ConsumeStaminaRate * DeltaTime;

	if (Cast<APlayerCharacter>(GetOwner())->CheckStopRecoveryStamina())
	{
	}
	/** 스테미나 사용중인 경우(캐릭터가 달리고 있는 경우) */
	else if (Cast<APlayerCharacter>(GetOwner())->CheckPlayConsumeStamina())
	{
		if (CurrentStamina - DeltaConsumeStamina <= 0.0f)
		{
			Cast<APlayerCharacter>(GetOwner())->ReleasedLShift();
		}
		else
		{
			DecCurrentStamina(DeltaConsumeStamina);
		}
	}
	/** 스테미나를 사용중이지 않을 경우 */
	else
	{
		/** 현재 스테미나를 MAXStamina가 될 때 까지 회복 */
		if (CurrentStamina <= MaxStamina)
		{
			IncCurrentStamina(DeltaStamina * 2);
		}
	}
}

void UCharacterStatComponent::LevelUp()
{
	if (Level < MaxLevel)
	{
		Level++;
		StatPoint += 5;

		OnLevelChanged.Broadcast();
		OnStatPointChanged.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("이미 최대 레벨에 도달을 하였습니다."));
	}
}

void UCharacterStatComponent::IncHP(int32 IncPoint)
{
	int32 NewHPPoint = HP + IncPoint;
	if (NewHPPoint <= MaxHP)
	{
		HP = NewHPPoint;
		IncMaxHealth(MaxHealthPerOneHP*IncPoint);
		OnHPStatChanged.Broadcast();
	}
}

void UCharacterStatComponent::IncSP(int32 IncPoint)
{
	int32 NewSPPoint = SP + IncPoint;
	if (NewSPPoint <= MaxSP)
	{
		SP = NewSPPoint;
		IncMaxStamina(MaxStaminaPerOneSP*IncPoint);
		OnSPStatChanged.Broadcast();
	}
}

void UCharacterStatComponent::IncSTR(int32 IncPoint)
{
	int32 NewSTRPoint = STR + IncPoint;
	if (NewSTRPoint <= MaxSTR)
	{
		STR = NewSTRPoint;
		IncAttack(AttackPerOneSTR*IncPoint);
		OnSTRStatChanged.Broadcast();
	}
}

void UCharacterStatComponent::IncAGI(int32 IncPoint)
{
	int32 NewAGIPoint = AGI + IncPoint;
	if (NewAGIPoint <= MaxAGI)
	{
		AGI = NewAGIPoint;
		IncSprintSpeed(SpeedPerOneAGI*IncPoint);
		OnAGIStatChanged.Broadcast();
	}
}

void UCharacterStatComponent::IncCON(int32 IncPoint)
{
	int32 NewCONPoint = CON + IncPoint;
	if (NewCONPoint <= MaxCON)
	{
		CON = NewCONPoint;
		OnCONStatChanged.Broadcast();
	}
}

void UCharacterStatComponent::DecHP(int32 DecPoint)
{
	int32 NewHPPoint = HP - DecPoint;
	if (NewHPPoint > 0)
	{
		HP = NewHPPoint;
	}
	else
	{
		HP = 0;
	}
	DecMaxHealth(MaxHealthPerOneHP*DecPoint);
	OnHPStatChanged.Broadcast();
}

void UCharacterStatComponent::DecSP(int32 DecPoint)
{
	int32 NewSPPoint = SP - DecPoint;
	if (NewSPPoint > 0)
	{
		SP = NewSPPoint;
	}
	else
	{
		SP = 0;
	}
	DecMaxStamina(MaxStaminaPerOneSP*DecPoint);
	OnSPStatChanged.Broadcast();
}

void UCharacterStatComponent::DecSTR(int32 DecPoint)
{
	int32 NewSTRPoint = STR - DecPoint;
	if (NewSTRPoint > 0)
	{
		STR = NewSTRPoint;
	}
	else
	{
		STR = 0;
	}
	DecAttack(AttackPerOneSTR*DecPoint);
	OnSTRStatChanged.Broadcast();
}

void UCharacterStatComponent::DecAGI(int32 DecPoint)
{
	int32 NewAGIPoint = AGI - DecPoint;
	if (NewAGIPoint > 0)
	{
		AGI = NewAGIPoint;
	}
	else
	{
		AGI = 0;
	}
	DecSprintSpeed(SpeedPerOneAGI*DecPoint);
	OnAGIStatChanged.Broadcast();
}

void UCharacterStatComponent::DecCON(int32 DecPoint)
{
	int32 NewCONPoint = CON - DecPoint;
	if (NewCONPoint > 0)
	{
		CON = NewCONPoint;
	}
	else
	{
		CON = 0;
	}
	OnCONStatChanged.Broadcast();
}

void UCharacterStatComponent::IncMaxHealth(float IncFigure)
{
	MaxHealth += IncFigure;
	OnMaxHealthChanged.Broadcast();
}

int32 UCharacterStatComponent::IncCurrentHealth(float IncFigure)
{
	int32 PreCurrentHealth = CurrentHealth + IncFigure;
	if (PreCurrentHealth >= MaxHealth)
	{
		int32 IncAmount = MaxHealth - CurrentHealth;
		CurrentHealth = MaxHealth;
		OnCurrentHealthChanged.Broadcast();
		return IncAmount;
	}
	CurrentHealth += IncFigure;
	OnCurrentHealthChanged.Broadcast();
	return IncFigure;
}

void UCharacterStatComponent::IncMaxStamina(float IncFigure)
{
	MaxStamina += IncFigure;
	OnMaxStaminaChanged.Broadcast();
}

void UCharacterStatComponent::IncCurrentStamina(float IncFigure)
{
	CurrentStamina += IncFigure;
	if (CurrentStamina >= MaxStamina)
	{
		CurrentStamina = MaxStamina;
	}
	OnCurrentStaminaChanged.Broadcast();
}

void UCharacterStatComponent::IncAttack(int32 IncFigure)
{
	Attack = Attack + IncFigure;
	OnAttackChanged.Broadcast();
}

void UCharacterStatComponent::IncDefense(int32 IncFigure)
{
	Defense += IncFigure;
	OnDefenseChanged.Broadcast();
}

void UCharacterStatComponent::IncSprintSpeed(int32 IncFigure)
{
	SprintSpeed += IncFigure;
}

void UCharacterStatComponent::DecMaxHealth(float DecFigure)
{
	float NewMaxHealth = MaxHealth - DecFigure;
	if (NewMaxHealth > 0)
	{
		MaxHealth = NewMaxHealth;
	}
	else
	{
		MaxHealth = 0;
	}
	OnMaxHealthChanged.Broadcast();

	if (MaxHealth < CurrentHealth)
	{
		CurrentHealth = MaxHealth;
		OnCurrentHealthChanged.Broadcast();
	}
}

void UCharacterStatComponent::DecCurrentHealth(float DecFigure)
{
	float NewCurrentHealth = CurrentHealth - DecFigure;
	if (NewCurrentHealth > 0)
	{
		CurrentHealth = NewCurrentHealth;
	}
	else
	{
		CurrentHealth = 0;
		OnCurrentHealthZero.Broadcast();
	}
	OnCurrentHealthChanged.Broadcast();
}

void UCharacterStatComponent::DecMaxStamina(float DecFigure)
{
	float NewMaxStamina = MaxStamina - DecFigure;
	if (NewMaxStamina > 0)
	{
		MaxStamina = NewMaxStamina;
	}
	else
	{
		MaxStamina = 0;
	}
	OnMaxStaminaChanged.Broadcast();

	if (MaxStamina < CurrentStamina)
	{
		CurrentStamina = MaxStamina;
		OnCurrentStaminaChanged.Broadcast();
	}
}

bool UCharacterStatComponent::DecCurrentStamina(float DecFigure)
{
	float NewCurrentStamina = CurrentStamina - DecFigure;
	if (NewCurrentStamina > 0)
	{
		CurrentStamina = NewCurrentStamina;
		OnCurrentStaminaChanged.Broadcast();
		return true;
	}
	else
	{
		CurrentStamina = 0;
		OnCurrentStaminaZero.Broadcast();
		OnCurrentStaminaChanged.Broadcast();
		return false;
	}
}

void UCharacterStatComponent::DecAttack(int32 DecFigure)
{
	int32 NewAttack = Attack - DecFigure;
	if (NewAttack > 0)
	{
		Attack = NewAttack;
	}
	else
	{
		Attack = 0;
	}
	OnAttackChanged.Broadcast();
}

void UCharacterStatComponent::DecDefense(int32 DecFigure)
{
	int32 NewDefense = Defense - DecFigure;
	if (NewDefense > 0)
	{
		Defense = NewDefense;
	}
	else
	{
		Defense = 0;
	}
	OnDefenseChanged.Broadcast();
}

void UCharacterStatComponent::DecSprintSpeed(int32 DecFigure)
{
	int32 NewSprintSpeed = SprintSpeed - DecFigure;
	if (NewSprintSpeed > NormalSpeed)
	{
		SprintSpeed = NewSprintSpeed;
	}
	else
	{
		SprintSpeed = NormalSpeed;
	}
}

bool UCharacterStatComponent::DecStaminaForEva(float DecFigure)
{
	float NewCurrentStamina = CurrentStamina - (DecFigure - (AGI * DecEvaPerOneAGI));
	if (NewCurrentStamina > 0)
	{
		CurrentStamina = NewCurrentStamina;
		OnCurrentStaminaChanged.Broadcast();
		return true;
	}
	else
	{
		return false;
	}
}

void UCharacterStatComponent::IncStatForItem(FName ItemName)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(ItemName);

	IncAttack(ItemData->GetDamage());
	IncDefense(ItemData->GetArmor());
	IncHP(ItemData->GetHP());
	IncSP(ItemData->GetSP());
	IncSTR(ItemData->GetSTR());
	IncAGI(ItemData->GetAGI());
	IncCON(ItemData->GetCON());
}

void UCharacterStatComponent::DecStatForItem(FName ItemName)
{
	UABGameInstance* ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemData* ItemData = ABGameInstance->GetItemData(ItemName);

	DecAttack(ItemData->GetDamage());
	DecDefense(ItemData->GetArmor());
	DecHP(ItemData->GetHP());
	DecSP(ItemData->GetSP());
	DecSTR(ItemData->GetSTR());
	DecAGI(ItemData->GetAGI());
	DecCON(ItemData->GetCON());
}

void UCharacterStatComponent::UseStatPointForHP()
{
	IncHP(1);
	BaseHP++;
	StatPoint--;
	OnStatPointChanged.Broadcast();
}

void UCharacterStatComponent::UseStatPointForSP()
{
	IncSP(1);
	BaseSP++;
	StatPoint--;
	OnStatPointChanged.Broadcast();
}

void UCharacterStatComponent::UseStatPointForSTR()
{
	IncSTR(1);
	BaseSTR++;
	StatPoint--;
	OnStatPointChanged.Broadcast();
}

void UCharacterStatComponent::UseStatPointForAGI()
{
	IncAGI(1);
	BaseAGI++;
	StatPoint--;
	OnStatPointChanged.Broadcast();
}

void UCharacterStatComponent::InitStat()
{
	Level = 1;
	
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	
	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;

	BaseHP = 0;
	BaseSP = 0;
	BaseSTR = 0;
	BaseAGI = 0;

	HP = 0;
	SP = 0;
	STR = 0;
	AGI = 0;
	CON = 0;
	StatPoint = 0;

	Attack = 0;
	Defense = 0;

	OnLevelChanged.Broadcast();
	OnMaxHealthChanged.Broadcast();
	OnCurrentHealthChanged.Broadcast();
	OnMaxStaminaChanged.Broadcast();
	OnCurrentStaminaChanged.Broadcast();

	OnAttackChanged.Broadcast();
	OnDefenseChanged.Broadcast();
	
}

void UCharacterStatComponent::SetCurrentSPToZero()
{
	CurrentStamina = 0;
	OnCurrentStaminaZero.Broadcast();
	OnCurrentStaminaChanged.Broadcast();
}

void UCharacterStatComponent::SaveCharacterStatInfo(UABSaveGame* SaveInstance)
{
	SaveInstance->Level = Level;
	SaveInstance->MaxHealth = 100.f;
	SaveInstance->CurrentHealth = CurrentHealth;
	SaveInstance->MaxStamina = 100.f;
	SaveInstance->CurrentStamina = CurrentStamina;
	SaveInstance->HP = BaseHP;
	SaveInstance->SP = BaseSP;
	SaveInstance->STR = BaseSTR;
	SaveInstance->AGI = BaseAGI;
	SaveInstance->CON = BaseCON;
	SaveInstance->StatPoint = StatPoint;
}

void UCharacterStatComponent::LoadCharacterStatInfo(UABSaveGame* LoadInstance)
{
	Level = LoadInstance->Level;
	MaxHealth = LoadInstance->MaxHealth;
	CurrentHealth = LoadInstance->CurrentHealth;
	MaxStamina = LoadInstance->MaxStamina;
	CurrentStamina = LoadInstance->CurrentStamina;

	BaseHP = LoadInstance->HP;
	IncHP(BaseHP);
	BaseSP = LoadInstance->SP;
	IncSP(BaseSP);
	BaseSTR = LoadInstance->STR;
	IncSTR(BaseSTR);
	BaseAGI = LoadInstance->AGI;
	IncAGI(BaseAGI);
	BaseCON = LoadInstance->CON;
	StatPoint = LoadInstance->StatPoint;

	
	OnLevelChanged.Broadcast();
	OnMaxHealthChanged.Broadcast();
	OnCurrentHealthChanged.Broadcast();
	OnMaxStaminaChanged.Broadcast();
	OnCurrentStaminaChanged.Broadcast();
	OnStatPointChanged.Broadcast();
	OnAttackChanged.Broadcast();
	OnDefenseChanged.Broadcast();
}