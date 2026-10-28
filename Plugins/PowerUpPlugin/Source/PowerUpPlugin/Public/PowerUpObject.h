#pragma once

#include "PowerUpObject.generated.h"

UENUM()
enum class EPowerUp : uint8
{
	Speed UMETA(DisplayName = "Speed"),
	Smash UMETA(DisplayName = "Smash"),
	Magnet UMETA(DisplayName = "Magnet"),
};

UCLASS()
class POWERUPPLUGIN_API UPowerUpObject : public UObject
{
	GENERATED_BODY()

public:
	UPowerUpObject();
	EPowerUp GetType() const;

private:
	EPowerUp Type{};
};
