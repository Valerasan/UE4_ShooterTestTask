// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterLauncher : public AShooterBaseWeapon
{
	GENERATED_BODY()

public:
	AShooterLauncher();

	virtual void BeginPlay() override;


	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AShooterLauncherProjectile> ProjectileClass;

	virtual void MakeShot() override;
	
};
