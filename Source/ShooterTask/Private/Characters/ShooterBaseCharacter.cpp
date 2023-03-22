// Fill out your copyright notice in the Description page of Project Settings.




// ShootThemUp All Rights Reserved

#include "Characters/ShooterBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Component/ShooterWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");


}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GetMesh());

	//OnHealthChanged(HealthComponent->GetHealt(), 0.f);
	//HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	//HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	//LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);


}

// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



bool AShooterBaseCharacter::IsRunning() const
{
	return false;
}

float AShooterBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AShooterBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

	if (!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}



void AShooterBaseCharacter::OnStartFire()
{
	if (IsRunning()) return;
	//WeaponComponent->StartFire();
}

void AShooterBaseCharacter::OnDeath()
{
	//UE_LOG(LogBaseCharacter, Display, TEXT("Is dead"));
	//PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//WeaponComponent->StopFire();
	//WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void AShooterBaseCharacter::OnHealthChanged(float Health, float HealthDeltd)
{

}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FallDamage, FPointDamageEvent{}, nullptr, nullptr);

	UE_LOG(LogBaseCharacter, Display, TEXT("Player %s recived landed damage: %f"), *GetName(), FallDamage);
}



