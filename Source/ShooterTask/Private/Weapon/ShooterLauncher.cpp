// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterLauncher.h"
#include "Weapon/ShooterLauncherProjectile.h"
#include "NiagaraComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStaticsTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogLauncher, All, All);


AShooterLauncher::AShooterLauncher()
{

}


void AShooterLauncher::StartFire()
{
	MakeShot();
}

void AShooterLauncher::MakeShot()
{
	if (!GetWorld()) return;

	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}

	FVector TraceStart, TraceEnd;

	if (!GetTraceDate(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	AShooterLauncherProjectile* Projectile = GetWorld()->SpawnActorDeferred<AShooterLauncherProjectile>(ProjectileClass, SpawnTransform);
	// Set projectile params
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreasAmmo();
	SpawnMuzzleFX();

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}
