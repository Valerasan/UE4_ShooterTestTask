// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERTASK_API UShooterWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShooterWeaponFXComponent();

	void PlayInpactFX(const FHitResult& Hit);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
