// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/ShooterNeedWeaponBTDecorator.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"

UShooterNeedWeaponBTDecorator::UShooterNeedWeaponBTDecorator()
{
	NodeName = "Weapon Pickup";
}

// check if the character needs a weapon
bool UShooterNeedWeaponBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	const auto Result = WeaponComponent->ExistsWeapon(WeaponType);

	return !Result;
}

