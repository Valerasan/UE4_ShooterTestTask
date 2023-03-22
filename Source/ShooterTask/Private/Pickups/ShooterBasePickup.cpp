// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ShooterBasePickup.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

AShooterBasePickup::AShooterBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisisonComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void AShooterBasePickup::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);

	GenerateRotationYaw();
}

void AShooterBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto PAwn = Cast<APawn>(OtherActor);
	//UE_LOG(LogBasePickup, Display, TEXT("Pickup was taken"));
	if (GivePikupTo(PAwn))
	{
		PickupWastaken();
	}

	// Destroy();
}

void AShooterBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
}

bool AShooterBasePickup::CouldBeTaken() const
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
	//return CouldBeTakenTest;
}

bool AShooterBasePickup::GivePikupTo(APawn* PlayrPawn)
{
	return false;
}

void AShooterBasePickup::PickupWastaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShooterBasePickup::Respawn, RespawnTime);
}

void AShooterBasePickup::Respawn()
{
	GenerateRotationYaw();
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}


void AShooterBasePickup::GenerateRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.f : -1.f;
	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}
