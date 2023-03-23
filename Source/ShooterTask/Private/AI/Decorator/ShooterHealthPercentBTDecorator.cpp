// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/ShooterHealthPercentBTDecorator.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterHealthComponent.h"

UShooterHealthPercentBTDecorator::UShooterHealthPercentBTDecorator()
{
	NodeName = "Health Percent";
}
// check if the character needs health
bool UShooterHealthPercentBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto HealthComponent = ShooterUtils::GetShooterPlayerComponent<UShooterHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}
