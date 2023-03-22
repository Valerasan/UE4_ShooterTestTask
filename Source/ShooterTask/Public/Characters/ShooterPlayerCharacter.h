// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ShooterBaseCharacter.h"
#include "ShooterPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterPlayerCharacter : public AShooterBaseCharacter
{
	GENERATED_BODY()


public:
	AShooterPlayerCharacter();

protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USphereComponent* CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	bool bIsMovingForward = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

};
