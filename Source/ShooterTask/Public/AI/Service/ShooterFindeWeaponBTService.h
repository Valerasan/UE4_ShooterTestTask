// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterFindeWeaponBTService.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterFindeWeaponBTService : public UBTService
{
	GENERATED_BODY()

public:
	UShooterFindeWeaponBTService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector WeaponLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<class AShooterBaseWeapon> WeaponType;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
