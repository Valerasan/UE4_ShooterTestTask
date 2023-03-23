// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShooterGameModeBase.h"
#include "Characters/ShooterBaseCharacter.h"
#include "Characters/ShooterPlayerController.h"
#include "AIController.h"
#include "ShooterUtils.h"
#include "Component/ShooterRespawnComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

AShooterGameModeBase::AShooterGameModeBase()
{
	DefaultPawnClass = AShooterBaseCharacter::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();

}

void AShooterGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
}

// returns pawn AIcontroller
UClass* AShooterGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnCalss;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AShooterGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < 3; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);
	}
}

void AShooterGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
}

// when any character dies, the respawn timer starts up
void AShooterGameModeBase::Killed(AController* VictimController)
{
	StartRespawn(VictimController);
}

// make respawn request in ShooterHealComponent.cpp
void AShooterGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

// starts the respawn process
void AShooterGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnComponent = ShooterUtils::GetShooterPlayerComponent<UShooterRespawnComponent>(Controller);

	if (!RespawnComponent) return;

	RespawnComponent->Respawn(10.f);
}
