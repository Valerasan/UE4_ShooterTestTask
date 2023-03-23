// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/ShooterChangeWeaponBTService.h"
#include "Component/ShooterWeaponComponent.h"
#include "ShooterUtils.h"
#include "AIController.h"

UShooterChangeWeaponBTService::UShooterChangeWeaponBTService()
{
	NodeName = "Change Weapon";
}

void UShooterChangeWeaponBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();

	if (Controller)
	{
		const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller->GetPawn());
		
		// with a chance to change weapons
		if (WeaponComponent && Probaility > 0 && FMath::FRand() <= Probaility)
		{
			WeaponComponent->NextWeapon();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
