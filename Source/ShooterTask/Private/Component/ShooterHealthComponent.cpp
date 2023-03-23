// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShooterHealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameMode/ShooterGameModeBase.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UShooterHealthComponent::UShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UShooterHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsHealthFull() || IsDead()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

// Called when the game starts
void UShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	check(MaxHealth > 0);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UShooterHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeRadialDamage);
	}
}

void UShooterHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealDelayTimerHandle);

	if (IsDead())
	{
		Killed();
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(
			HealDelayTimerHandle, this, &UShooterHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UShooterHealthComponent::HealUpdate()
{
	SetHealth(HealModifire + Health);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealDelayTimerHandle);
	}
}

void UShooterHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, HealthDelta);
}


bool UShooterHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UShooterHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	// UE_LOG(LogHealthComponent, Display, TEXT("DamageModifier: %f"), GetPointDamageModifier(DamagedActor, BoneName));
	const auto FinalDamage = Damage;
	UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());

	ApplyDamage(FinalDamage, InstigatedBy);
}

void UShooterHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
	FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);

	ApplyDamage(Damage, InstigatedBy);
}

//
void UShooterHealthComponent::Killed()
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());

	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(VictimController);
}