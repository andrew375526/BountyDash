#include "PowerUpObject.h"

UPowerUpObject::UPowerUpObject()
{
	switch (FMath::Rand() % 3)
	{
		using enum EPowerUp;
	case 0:
		Type = Speed;
		break;
	case 1:
		Type = Smash;
		break;
	case 2:
		Type = Magnet;
		break;
#if defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_DEBUG)
	default:
		check(!"Missing case.");
#endif
	}
}

EPowerUp UPowerUpObject::GetType() const
{
	return Type;
}
