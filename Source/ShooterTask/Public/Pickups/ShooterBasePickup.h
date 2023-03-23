// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBasePickup.generated.h"

UCLASS()
class SHOOTERTASK_API AShooterBasePickup : public AActor
{
	GENERATED_BODY()
	
public:
	AShooterBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float RespawnTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundCue* PickupSound;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		bool CouldBeTakenTest = true;

public:
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;

private:
	float RotationYaw = 0.0f;
	FTimerHandle RespawnTimerHandle;

	virtual bool GivePikupTo(APawn* PlayrPawn);
	void PickupWastaken();
	void Respawn();

	void GenerateRotationYaw();

};
