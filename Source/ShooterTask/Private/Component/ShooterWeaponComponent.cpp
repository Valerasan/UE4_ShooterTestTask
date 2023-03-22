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

	//checkf(WeaponData.Num() == WeaponNum, TEXT("Our chracter can hold only %i weapon items"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	//EquipWeapon(CurrentWeaponIndex);
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
		auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeapon>(OneWeaponData);
		if (!Weapon) return;

		Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Chracter);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Chracter->GetMesh(), WeaponArmorySocketName);
	}
}

void UShooterWeaponComponent::AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UShooterWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
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
		AttachWeaponToSocket(CurrentWeapon, Chracter->GetMesh(), WeaponArmorySocketName);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	//const auto CurrenWeaponData = WeaponData.FindByPredicate([&](const TSubclassOf<AShooterBaseWeapon>& Data) {	 //
	//	return Data.GetDefaultObject()->GetClass() == CurrentWeapon->GetClass();								 //
	//	});


	CurrentReloadAnimMontage =  CurrentWeapon->ReloadAnimMontage;

	AttachWeaponToSocket(CurrentWeapon, Chracter->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);

}

void UShooterWeaponComponent::StartFire()
{
	const auto Actor = Cast <AShooterPlayerCharacter>(GetOwner());
	if (!CanFire() || Actor->IsRunning()) return;
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

bool UShooterWeaponComponent::TryToAddWeapon(TSubclassOf<AShooterBaseWeapon> WeaponType)
{


	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || !GetWorld()) return false;

	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return false;
		}
	}
	auto TryWeapon = GetWorld()->SpawnActor<AShooterBaseWeapon>(WeaponType);
	if (!TryWeapon) return false;

	TryWeapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
	TryWeapon->SetOwner(Chracter);
	Weapons.Add(TryWeapon);

	AttachWeaponToSocket(TryWeapon, Chracter->GetMesh(), WeaponArmorySocketName);
	
	auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterRealodAnimNotify>(TryWeapon->ReloadAnimMontage);
	if (!ReloadFinishedNotify)
	{
		//UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}
	ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);


	return true;
}

void UShooterWeaponComponent::Zoom(bool Enabled)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}

void UShooterWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter) return;

	Chracter->PlayAnimMontage(Animation);
}

void UShooterWeaponComponent::InitAnimations()
{

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

	for (TSubclassOf<AShooterBaseWeapon> OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterRealodAnimNotify>(OneWeaponData.GetDefaultObject()->ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			//UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);
	}
}

void UShooterWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || Chracter->GetMesh() != MeshComp) return;

	//OnEmptyClip(CurrentWeapon);

	EquipAnimInProgress = false;
	//UE_LOG(LogWeaponComponent, Warning, TEXT("Equip finished"));
	CurrentWeapon->IsNeedToReload();
}

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
	UE_LOG(LogWeaponComponent, Warning, TEXT("============================"));
	UE_LOG(LogWeaponComponent, Warning, TEXT("Equip: %s"), EquipAnimInProgress ? TEXT("true") : TEXT("false"));
	UE_LOG(LogWeaponComponent, Warning, TEXT("Reload: %s"), ReloadAnimInProgress ? TEXT("true") : TEXT("false"));
	UE_LOG(LogWeaponComponent, Warning, TEXT("Can Reload: %s"), CurrentWeapon->CanReload() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogWeaponComponent, Warning, TEXT("============================"));


	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

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

void UShooterWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}
