// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterLauncherProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Component/ShooterWeaponFXComponent.h"

AShooterLauncherProjectile::AShooterLauncherProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpehreComponent");
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.f;

	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>("WeaponFXComponent");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

void AShooterLauncherProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AShooterLauncherProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void AShooterLauncherProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	// make damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(),	 //
		DamageAmount,								 //
		GetActorLocation(),							 //
		DamageRadius,								 //
		UDamageType::StaticClass(),					 //
		{ GetOwner() },								 //
		this,										 //
		GetControllerChracter(),					 //
		DoFullDamage);


	WeaponFXComponent->PlayInpactFX(Hit);

	SetLifeSpan(0.8f);
}

AController* AShooterLauncherProjectile::GetControllerChracter() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}


