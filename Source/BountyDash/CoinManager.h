#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "CoinManager.generated.h"
/**
 *
 */
UCLASS()
class BOUNTYDASH_API UCoinManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void AddCoin(class ACoin*);
	const TArray<TWeakObjectPtr<class ACoin>>& GetCoins() const;

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<class ACoin>> Coins{};
};
