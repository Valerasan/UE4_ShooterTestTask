// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterAmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterAmmoPickup : public AShooterBasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", ClampMax = "10"))
		int32 ClipsAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		TSubclassOf<class AShooterBaseWeapon> WeaponType;

private:
	virtual bool GivePikupTo(APawn* PlayrPawn) override;
	
};
