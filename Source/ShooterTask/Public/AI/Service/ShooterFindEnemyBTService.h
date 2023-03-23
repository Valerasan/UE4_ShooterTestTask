// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterFindEnemyBTService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterFindEnemyBTService : public UBTService
{
	GENERATED_BODY()

public:
	UShooterFindEnemyBTService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
