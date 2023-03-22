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

	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AShooterLauncherProjectile> ProjectileClass;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	// UNiagaraComponent* Niagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* NoAmmoSound;

	virtual void MakeShot() override;
	
};
