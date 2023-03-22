// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Component/ShooterWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
UShooterWeaponFXComponent::UShooterWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UShooterWeaponFXComponent::PlayInpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),	//
		ImpactData.NiagaraSystem,								//
		Hit.ImpactPoint,										//
		Hit.ImpactNormal.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactData.ImpactSound, Hit.ImpactPoint);

	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  //
		ImpactData.DecalData.Material,										  //
		ImpactData.DecalData.Size,											  //
		Hit.ImpactPoint,													  //
		Hit.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime,	//
			ImpactData.DecalData.FadeOutTime);
	}
}

