// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERTASK_API AShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShooterGameModeBase();


	virtual void StartPlay();
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed( AController* VictimController);

	void RespawnRequest(AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
		TSubclassOf<class AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		TSubclassOf<APawn> AIPawnCalss;


private:

	void SpawnBots();

	void ResetOnePlayer(AController* Controller);

	void StartRespawn(AController* Controller);

	
};
