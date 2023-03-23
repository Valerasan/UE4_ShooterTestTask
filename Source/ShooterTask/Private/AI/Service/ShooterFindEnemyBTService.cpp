// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/ShooterFindEnemyBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "AI/ShooterAIPerceptionComponent.h"

UShooterFindEnemyBTService::UShooterFindEnemyBTService()
{
	NodeName = "Find Enemy";
}

void UShooterFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//UE_LOG(LogTemp, Display, TEXT("Find Enemy"));
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		 
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionCompontn = ShooterUtils::GetShooterPlayerComponent<UShooterAIPerceptionComponent>(Controller);
		if (PerceptionCompontn)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionCompontn->GetClossestEnemy());
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
