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

void AShooterLauncher::BeginPlay()
{
	Super::BeginPlay();
	MakeShoot.AddUObject(this, &AShooterLauncher::MakeShot);
}


void AShooterLauncher::StartFire()
{
	Super::StartFire();
	UE_LOG(LogTemp, Warning, TEXT("Launcher Fire"));

}

void AShooterLauncher::StopFire()
{
	Super::StopFire();
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

	if (CurrentAmmo.Clips == 0)
	{
		UGameplayStatics::SpawnSoundAttached(LastBullet, WeaponMesh, MuzzleSocketName);
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
}
