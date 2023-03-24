// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponComponent.generated.h"

class AShooterBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERTASK_API UShooterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UShooterWeaponComponent();

	virtual void StartFire();
	void StopFire();
	bool IsFiring() const;

	virtual void NextWeapon();
	virtual void PreviousWeapon();
	void Reload();
	
	// TODO
	//bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType);

	bool TryToAddWeapon(TSubclassOf<AShooterBaseWeapon> WeaponType);

	//TODO
	//void Zoom(bool Enabled);
	bool ExistsWeapon(TSubclassOf<AShooterBaseWeapon> CheckWeapon);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AShooterBaseWeapon>> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		UAnimMontage* EquipAnimMontage;

	UPROPERTY()
		class AShooterBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<AShooterBaseWeapon*> Weapons;

	int32 CurrentWeaponIndex = 0;

	bool ReloadAnimInProgress = false;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(int32 WeaponIndex);
	
private:
	UPROPERTY()
		UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool EquipAnimInProgress = false;
	// if it is false, then there is only one weapon in the hand 
	// and the animation of the equium does not work
	bool OneWeapon = false;

	void SpawnWeapons();
	void SpawnWeapon(const TSubclassOf<AShooterBaseWeapon>& SpawnedWeapon);

	void AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	bool CanReload() const;

	void OnEmptyClip(AShooterBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
		
};
