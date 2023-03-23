// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/ShooterFindeWeaponBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterUtils.h"
#include "AIController.h"
#include "Component/ShooterWeaponComponent.h"

UShooterFindeWeaponBTService::UShooterFindeWeaponBTService()
{
	NodeName = "Find Weapon";
}

void UShooterFindeWeaponBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//UE_LOG(LogTemp, Display, TEXT("Find Enemy"));
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{

		const auto Controller = OwnerComp.GetAIOwner();
		const auto WeaponCompontn = ShooterUtils::GetShooterPlayerComponent<UShooterWeaponComponent>(Controller);
		if (!WeaponCompontn->ExistsWeapon(WeaponType))
		{
			//Blackboard->SetValueAsObject(WeaponLocationKey.SelectedKeyName, PerceptionCompontn->GetClossestEnemy());
		}

	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}