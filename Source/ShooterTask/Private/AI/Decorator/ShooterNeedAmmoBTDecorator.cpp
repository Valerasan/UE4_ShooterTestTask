// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/ShooterNeedAmmoBTDecorator.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"

UShooterNeedAmmoBTDecorator::UShooterNeedAmmoBTDecorator()
{
    NodeName = "Ammo Pickup";
}
// check if the character needs ammunition
bool UShooterNeedAmmoBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}
