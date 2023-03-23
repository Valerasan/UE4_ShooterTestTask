// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/ShooterFireBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterWeaponComponent.h"

UShooterFireBTService::UShooterFireBTService()
{
	NodeName = "Fire";
}

void UShooterFireBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			// if there is a focus, it will shoot
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
