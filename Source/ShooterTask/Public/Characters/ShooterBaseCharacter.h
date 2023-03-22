// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

UCLASS()
class SHOOTERTASK_API AShooterBaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	//AShooterBaseCharacter(const FObjectInitializer& ObjInit);
	AShooterBaseCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UShooterWeaponComponent* WeaponComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Material")
		FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundCue* DeathSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health, float HealthDelta);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color);

private:
	void OnStartFire();

	UFUNCTION()
		void OnGroundLanded(const FHitResult& Hit);
};
