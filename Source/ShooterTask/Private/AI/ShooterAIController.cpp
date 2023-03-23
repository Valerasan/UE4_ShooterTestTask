// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"
#include "AI/ShooterAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/ShooterRespawnComponent.h"

AShooterAIController::AShooterAIController()
{
	ShooterAIPerceptionComponent = CreateDefaultSubobject<UShooterAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*ShooterAIPerceptionComponent);

	ShooterRespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("STURespawnComponent");
}

// starts behavioral tree
void AShooterAIController::OnPossess(APawn* InPawn)
{
	const auto ShooterCharacter = Cast<AShooterAICharacter>(InPawn);
	if (ShooterCharacter)
	{
		RunBehaviorTree(ShooterCharacter->BehaviorTreeAsset);
	}
	Super::OnPossess(InPawn);
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// will turn to the actor that has focus
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* AShooterAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FoucsOnKeyName));

}

