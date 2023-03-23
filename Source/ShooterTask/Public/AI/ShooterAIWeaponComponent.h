// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ShooterWeaponComponent.h"
#include "ShooterAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API UShooterAIWeaponComponent : public UShooterWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void NextWeapon() override;
	
};
