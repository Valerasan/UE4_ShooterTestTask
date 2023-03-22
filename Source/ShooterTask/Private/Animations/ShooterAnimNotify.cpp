// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ShooterAnimNotify.h"

void UShooterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
