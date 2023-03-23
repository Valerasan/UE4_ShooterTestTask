// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShooterPlayerController.h"
#include "Component/ShooterRespawnComponent.h"
#include "GameMode/ShooterGameModeBase.h"


AShooterPlayerController::AShooterPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>("RespawnComponent");
}