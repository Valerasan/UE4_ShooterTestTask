#pragma once
#include "ShooterCoreTypes.generated.h"
// weapon



DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, class AShooterBaseWeapon*)

USTRUCT(BlueprintType) struct FAmmoData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
		int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool Infinite;
};




USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FVector Size = FVector(10.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float LifeTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FDecalData DecalData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* ImpactSound;
};