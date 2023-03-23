// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterChangeWeaponBTService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterChangeWeaponBTService : public UBTService
{
	GENERATED_BODY()

public:
	UShooterChangeWeaponBTService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0", ClampMax = "1"))
		float Probaility = 0.8f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
