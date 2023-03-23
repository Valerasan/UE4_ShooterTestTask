// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterRifle.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Component/ShooterWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifle, All, All);

AShooterRifle::AShooterRifle()
{
	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>("WeaponFXComponent");
}

void AShooterRifle::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);

	MakeShoot.AddUObject(this, &AShooterRifle::MakeShot);
}

void AShooterRifle::StartFire()
{
	InitFX();
	Super::StartFire();
	//GetWorldTimerManager().SetTimer(ShotTimerHandl, this, &AShooterRifle::MakeShot, TimeBetweenShots, true);

	UE_LOG(LogRifle, Warning, TEXT("Rifle Fire"));

}

void AShooterRifle::StopFire()
{
	Super::StopFire();
	SetFXActive(false);
}

void AShooterRifle::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetControllerChracter());
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (Enabled)
	{
		DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}

	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}

void AShooterRifle::MakeShot()
{
	// UE_LOG(LogTemp, Display, TEXT("Make Shot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;

	if (!GetTraceDate(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	///////////////////////////
	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector ActualTraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	const FVector HitDirectionFromMuzzle = (ActualTraceEnd - SocketTransform.GetLocation()).GetSafeNormal();
	const float Degrees =
		FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SocketTransform.GetRotation().GetForwardVector(), HitDirectionFromMuzzle)));
	// UE_LOG(LogBaseWeapon, Display, TEXT("Andle %f"), Degrees);
	/////////////////////////

	FVector TraceFXend = TraceEnd;

	if (HitResult.bBlockingHit)
	{
		TraceFXend = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayInpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXend);
	ChangeSound();
	DecreasAmmo();
	if (CurrentAmmo.Bullets == 0)
	{
		UGameplayStatics::SpawnSoundAttached(LastBullet, WeaponMesh, MuzzleSocketName);
	}
}

bool AShooterRifle::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotaion;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotaion)) return false;

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotaion.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AShooterRifle::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return;


	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamageActor->TakeDamage(DamageAmount, PointDamageEvent, GetControllerChracter(), this);
}

void AShooterRifle::InitFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	if (!FireAudioComponent)
	{
		if (CurrentAmmo.Bullets > LowAmmoSoundPercent)
		{
			
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
		}
		else
		{
			
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(LowAmmo, WeaponMesh, MuzzleSocketName);
		}
	}
	SetFXActive(true);
}

void AShooterRifle::SetFXActive(bool IsActive)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}

	if (FireAudioComponent)
	{
		// IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
		FireAudioComponent->SetPaused(!IsActive);
	}
}

void AShooterRifle::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

void AShooterRifle::ChangeSound()
{
	if (FireAudioComponent)
	{
		if (CurrentAmmo.Bullets > LowAmmoSoundPercent)
		{
			
			if (FireAudioComponent->Sound->GetFName() == FireSound->GetFName())
			{
				UE_LOG(LogRifle, Display, TEXT("Fire sound"));
				return;
			}
			SetFXActive(false);
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);

		}
		else
		{
			if (FireAudioComponent->Sound->GetFName() == LowAmmo->GetFName())
			{
				UE_LOG(LogRifle, Display, TEXT("LowAmmo"));
				return;
			}
			SetFXActive(false);
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(LowAmmo, WeaponMesh, MuzzleSocketName);
		}
	}
	SetFXActive(true);
}

AController* AShooterRifle::GetControllerChracter() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

