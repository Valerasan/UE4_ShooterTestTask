// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "ShooterPickupEnvQueryTest.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterPickupEnvQueryTest : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UShooterPickupEnvQueryTest(const FObjectInitializer& ObjectInitializer);
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	
};
