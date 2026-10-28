#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/ObjectPtr.h"

#include "BountyDashHUD.generated.h"

/**
 *
 */
UCLASS()
class BOUNTYDASH_API ABountyDashHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABountyDashHUD();
	void DrawHUD() override;

private:
	UPROPERTY()
	TObjectPtr<class UFont> HUDFont{};
};
