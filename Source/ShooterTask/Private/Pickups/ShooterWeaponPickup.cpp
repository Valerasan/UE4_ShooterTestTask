// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/ShooterWeaponPickup.h"
#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"

bool AShooterWeaponPickup::GivePikupTo(APawn* PlayrPawn)
{
	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(PlayrPawn);
	if (!WeaponComponent) return false;

	return WeaponComponent->TryToAddWeapon(Weapon);
}
