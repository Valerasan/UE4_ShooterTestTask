// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterWeaponPickup : public AShooterBasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", ClampMax = "100"))
	TSubclassOf<class AShooterBaseWeapon> Weapon;

private:
	virtual bool GivePikupTo(APawn* PlayrPawn) override;
};
