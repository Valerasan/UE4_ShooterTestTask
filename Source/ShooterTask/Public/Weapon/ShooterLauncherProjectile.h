// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterLauncherProjectile.generated.h"

UCLASS()
class SHOOTERTASK_API AShooterLauncherProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AShooterLauncherProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool DoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float LifeSeconds = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
		class UShooterWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;

private:
	FVector ShotDirection;

	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			const FHitResult& Hit);

	AController* GetControllerChracter() const;

};
