// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterCoreTypes.h"
#include "ShooterBaseWeapon.generated.h"



UCLASS()
class SHOOTERTASK_API AShooterBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:

	AShooterBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();
	bool IsFiring() const;

	void ChangeClip();
	void IsNeedToReload();

	bool CanReload() const;

	//FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetAmmoData() const { return CurrentAmmo; }

	bool TryToAddAmmo(int32 ClipsAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;

	virtual void Zoom(bool Enable) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* ReloadAnimMontage;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float TraceMaxDistance = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo {
		15, 10, false
	};

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		class UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* FireSound;

	

	virtual void BeginPlay() override;


	virtual void MakeShot();
	virtual bool GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;

	void MakeHit(FHitResult& HitResilt, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreasAmmo();

	bool IsClipEmpty() const;

	void LogAmmo();

	class UNiagaraComponent* SpawnMuzzleFX();

private:
	FAmmoData CurrentAmmo;

	bool FireInProgress = false;

};
