// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ShooterWeaponComponent.h"

#include "Weapon/ShooterBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/ShooterEquipAnimNotify.h"
#include "Animations/ShooterRealodAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Characters/ShooterPlayerCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 3;

UShooterWeaponComponent::UShooterWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == 1, TEXT("Our chracter must have one starting weapon"));

	CurrentWeaponIndex = 0;
	
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShooterEquipAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnEquipFinished);
	}
	else
	{
		//UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}



	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	OneWeapon = Weapons.Num() == 1 ? true : false;
}

void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void UShooterWeaponComponent::SpawnWeapons()
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || !GetWorld()) return;

	for (TSubclassOf<AShooterBaseWeapon> OneWeaponData : WeaponData)
	{
		SpawnWeapon(OneWeaponData);
	}
}

void UShooterWeaponComponent::SpawnWeapon(const TSubclassOf<AShooterBaseWeapon>& SpawnedWeapon)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || !GetWorld()) return;

	auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeapon>(SpawnedWeapon);
	if (!Weapon) return;

	Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
	Weapon->SetOwner(Chracter);
	Weapons.Add(Weapon);
	Weapon->SetActorHiddenInGame(true);

	auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterRealodAnimNotify>(Weapon->ReloadAnimMontage);
	if (!ReloadFinishedNotify)
	{
		//UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}
	ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);

}

void UShooterWeaponComponent::AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UShooterWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || OneWeapon)
	{
		//UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}
	
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(false);
		CurrentWeapon->StopFire();
		CurrentWeapon->SetActorHiddenInGame(true);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	//const auto CurrenWeaponData = WeaponData.FindByPredicate([&](const TSubclassOf<AShooterBaseWeapon>& Data) {	 //
	//	return Data.GetDefaultObject()->GetClass() == CurrentWeapon->GetClass();								 //
	//	});


	CurrentReloadAnimMontage =  CurrentWeapon->ReloadAnimMontage;
	AttachWeaponToSocket(CurrentWeapon, Chracter->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	CurrentWeapon->SetActorHiddenInGame(false);

	PlayAnimMontage(EquipAnimMontage);

}

void UShooterWeaponComponent::StartFire()
{
	const auto Actor = Cast <AShooterPlayerCharacter>(GetOwner());
	if (!CanFire()) return;
	CurrentWeapon->StartFire();

	//UE_LOG(LogWeaponComponent, Warning, TEXT("Fire"));

}

void UShooterWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

bool UShooterWeaponComponent::IsFiring() const
{
	return CurrentWeapon && CurrentWeapon->IsFiring();
}

void UShooterWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	UE_LOG(LogWeaponComponent, Warning, TEXT("Index: %i"), CurrentWeaponIndex);
	ReloadAnimInProgress = false;
	EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::PreviousWeapon()
{
	if (!CanEquip()) return;
	int32 Index = (CurrentWeaponIndex - 1) % Weapons.Num();
	CurrentWeaponIndex = Index == -1 ? Weapons.Num() - 1 : Index;
	UE_LOG(LogWeaponComponent, Warning, TEXT("Index: %i"), CurrentWeaponIndex);
	ReloadAnimInProgress = false;
	EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::Reload()
{
	ChangeClip();
}

//bool UShooterWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
//{
//	if (CurrentWeapon)
//	{
//		UIData = CurrentWeapon->GetUIData();
//		return true;
//	}
//	return false;
//}

bool UShooterWeaponComponent::GetAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool UShooterWeaponComponent::TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

// TODO AI need ammo
bool UShooterWeaponComponent::NeedAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

// if the same weapon is already in hand, then it will not pick it up, and if not, it will pick it up
bool UShooterWeaponComponent::TryToAddWeapon(TSubclassOf<AShooterBaseWeapon> WeaponType)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || !GetWorld()) return false;
	
	// checking if the same weapon
	if (ExistsWeapon(WeaponType)) return false;

	OneWeapon = false;
	SpawnWeapon(WeaponType);
	return true;
}

void UShooterWeaponComponent::Zoom(bool Enabled)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}

bool UShooterWeaponComponent::ExistsWeapon(TSubclassOf<AShooterBaseWeapon> CheckWeapon)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(CheckWeapon))
		{
			return true;
		}
	}
	return false;
}

void UShooterWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter) return;

	Chracter->PlayAnimMontage(Animation);
}


// call when anime Equip notify works
void UShooterWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || Chracter->GetMesh() != MeshComp) return;

	EquipAnimInProgress = false;
	//UE_LOG(LogWeaponComponent, Warning, TEXT("Equip finished"));
	CurrentWeapon->IsNeedToReload();
}
// call when anime Reload notify works
void UShooterWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || Chracter->GetMesh() != MeshComp) return;

	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = false;
	// UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
}

bool UShooterWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UShooterWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress;
}

bool UShooterWeaponComponent::CanReload() const
{
	//UE_LOG(LogWeaponComponent, Warning, TEXT("============================"));
	//UE_LOG(LogWeaponComponent, Warning, TEXT("Equip: %s"), EquipAnimInProgress ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogWeaponComponent, Warning, TEXT("Reload: %s"), ReloadAnimInProgress ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogWeaponComponent, Warning, TEXT("Can Reload: %s"), CurrentWeapon->CanReload() ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogWeaponComponent, Warning, TEXT("============================"));


	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

// call when OnEmptyClip broadcast
void UShooterWeaponComponent::OnEmptyClip(AShooterBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;
	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
		UE_LOG(LogWeaponComponent, Warning, TEXT("Change Clip"));
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

// reloads in hand
void UShooterWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}
