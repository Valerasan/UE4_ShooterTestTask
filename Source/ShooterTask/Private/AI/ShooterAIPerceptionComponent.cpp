// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ShooterAIPerceptionComponent.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"


AActor* UShooterAIPerceptionComponent::GetClossestEnemy() const
{
	TArray<AActor*> PerciveActors;
	// gets all the actors that come into view
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerciveActors);
	if (PerciveActors.Num() == 0)
	{
	    // gets all the actors from which the damage was received
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerciveActors);
		if (PerciveActors.Num() == 0) return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	// search for nearest actor
	for (const auto PercieveActor : PerciveActors)
	{
		const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(PercieveActor);

		const auto PercivePawn = Cast<APawn>(PercieveActor);

		if (HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}

	return BestPawn;
}
