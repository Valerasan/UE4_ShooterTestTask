// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"
#include "Pickups/ShooterWeaponPickup.h"

bool AShooterWeaponPickup::GivePikupTo(APawn* PlayrPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<UShooterWeaponComponent>(PlayrPawn);
	if (!HealthComponent) return false;

	return HealthComponent->TryToAddWeapon(Weapon);
}
