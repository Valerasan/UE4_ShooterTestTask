// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterHealthPickup : public AShooterBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", ClampMax = "100"))
		float HealthAmount = 50.f;

private:
	virtual bool GivePikupTo(APawn* PlayrPawn) override;
	
};
