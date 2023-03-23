// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/ShooterWeaponComponent.h"

AShooterPlayerCharacter::AShooterPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.f, 100.f, 80.f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArm);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);
}

void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	//check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &AShooterBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::PreviousWeapon);
	//PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);

	// TODO make zoom
	/*DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool)
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Zoom, true);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &USTUWeaponComponent::Zoom, false);*/
}

void AShooterPlayerCharacter::MoveForward(float Amount)
{
	bIsMovingForward = Amount > 0.f;
	if (Amount == 0.f) return;
	AddMovementInput(GetActorForwardVector(), Amount);

}

void AShooterPlayerCharacter::MoveRight(float Amount)
{
	if (Amount == 0.f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

// TODO
// call when chcracter is dead
void AShooterPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}