// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterNeedWeaponBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterNeedWeaponBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UShooterNeedWeaponBTDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<class AShooterBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
};
