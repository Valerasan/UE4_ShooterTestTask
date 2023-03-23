// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterRifle : public AShooterBaseWeapon
{
	GENERATED_BODY()
	
		AShooterRifle();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float BulletSpread = 1.5f;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UShooterWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FString TraceTargetName = "TraceTarget";

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const override;

private:
	FTimerHandle ShotTimerHandl;

	UPROPERTY()
	class UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
		UAudioComponent* FireAudioComponent;

	void MakeDamage(const FHitResult& HitResult);
	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
	void ChangeSound();
	AController* GetControllerChracter() const;

	float 	DefaultCameraFOV = 90.f;

};
