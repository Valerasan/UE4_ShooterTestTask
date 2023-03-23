// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShooterRespawnComponent.h"
#include "GameMode/ShooterGameModeBase.h"

UShooterRespawnComponent::UShooterRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UShooterRespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UShooterRespawnComponent::ResspawnTimerUpdate, 1.0f, true);
}

void UShooterRespawnComponent::ResspawnTimerUpdate()
{
	if (--RespawnCountDown == 0)
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());

		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

bool UShooterRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}
