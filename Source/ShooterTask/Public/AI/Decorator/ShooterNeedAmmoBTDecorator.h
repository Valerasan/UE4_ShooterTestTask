// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterNeedAmmoBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterNeedAmmoBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UShooterNeedAmmoBTDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<class AShooterBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
};
