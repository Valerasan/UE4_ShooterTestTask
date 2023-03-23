// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ShooterHealthPickup.h"
#include "Component/ShooterHealthComponent.h"
#include "ShooterUtils.h"

bool AShooterHealthPickup::GivePikupTo(APawn* PlayrPawn)
{
	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(PlayrPawn);
	if (!HealthComponent) return false;

	return HealthComponent->TryToAddHealth(HealthAmount);
}
