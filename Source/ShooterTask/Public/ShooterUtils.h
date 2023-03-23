#pragma once

class ShooterUtils
{

public:
	template <typename T>
	static T* GetShooterPlayerComponent(AActor* PlyaerPawn)
	{
		if (!PlyaerPawn) return nullptr;

		const auto Component = PlyaerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};