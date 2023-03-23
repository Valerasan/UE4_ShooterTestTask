// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ShooterAmmoPickup.h"
#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"

bool AShooterAmmoPickup::GivePikupTo(APawn* PlayrPawn)
{
	//const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<USTUHealthComponent>(PlayrPawn);
	//if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(PlayrPawn);
	if (!WeaponComponent) return false;


	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
