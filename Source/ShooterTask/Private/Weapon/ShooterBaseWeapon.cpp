// Fill out your copyright notice in the Description page of Project Settings.



// ShootThemUp All Rights Reserved

#include "Weapon/ShooterBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	SetRootComponent(WeaponMesh);
}

void AShooterBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn`t be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Bullets count couldn`t be less or equal zero"));

	FireInProgress = false;
	CurrentAmmo = DefaultAmmo;
}

void AShooterBaseWeapon::StartFire()
{
	FireInProgress = true;
}

void AShooterBaseWeapon::StopFire()
{
	FireInProgress = false;
}

bool AShooterBaseWeapon::IsFiring() const
{
	return FireInProgress;
}

void AShooterBaseWeapon::MakeShot() {}



bool AShooterBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{

	const auto STUCharacter = Cast<ACharacter>(GetOwner());
	if (!STUCharacter) return false;

	if (STUCharacter->IsPlayerControlled())
	{
		const auto Controller = STUCharacter->GetController<APlayerController>();
		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}

	return true;
}

FVector AShooterBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool AShooterBaseWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotaion;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotaion)) return false;

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotaion.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AShooterBaseWeapon::MakeHit(FHitResult& HitResilt, const FVector& TraceStart, const FVector& TraceEnd)
{

	if (!GetWorld()) return;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResilt, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AShooterBaseWeapon::DecreasAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		return;
	}

	CurrentAmmo.Bullets--;
	LogAmmo();
	IsNeedToReload();
}

bool AShooterBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}


bool AShooterBaseWeapon::IsClipEmpty() const
{

	return CurrentAmmo.Bullets == 0;
}

bool AShooterBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void AShooterBaseWeapon::ChangeClip()
{

	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

void AShooterBaseWeapon::IsNeedToReload()
{
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool AShooterBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets&& CurrentAmmo.Clips > 0;
}

bool AShooterBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if (IsAmmoEmpty())
	{
		//UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty"));
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipsAmmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmmount;
			//UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"));
		}
		else
		{
			//UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full"));
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		//UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"))
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}

	return true;
}

void AShooterBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
}

UNiagaraComponent* AShooterBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,  //
		WeaponMesh,												   //
		MuzzleSocketName,										   //
		FVector::ZeroVector,									   //
		FRotator::ZeroRotator,									   //
		EAttachLocation::SnapToTarget,							   //
		true);
}
