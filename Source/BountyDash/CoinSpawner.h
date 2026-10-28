// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectPtr.h"
#include "Traits/IsTSubclassOf.h"

#include "CoinSpawner.generated.h"

UCLASS()
class BOUNTYDASH_API ACoinSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoinSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnCoin();
	void SpawnCoinSet();
	void SpawnPowerUp();
	void MoveSpawner();
	int NumCoinsToSpawn{};
	int TargetLoc{};
	struct FTimerHandle CoinSetTimerHandle;
	struct FTimerHandle CoinTimerHandle;
	struct FTimerHandle SpawnerMoveTimerHandle;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACoin> CoinObject{};

	UPROPERTY()
	TObjectPtr<class USceneComponent> Root{};

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int MaxSetCoins{ 5 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int MinSetCoins{ 3 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	double CoinSetTimeInterval{ 4.0 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	double CoinTimeInterval{ 0.5 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	double SpawnerMovementTimeInterval{ 1.0 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABountyDashPowerUp> PowerUpObject{};

	UPROPERTY(EditAnywhere, meta = (
		ClampMin = "0.0",
		ClampMax = "100.0",
		UIMin = "0.0",
		UIMax = "100.0",
		AllowPrivateAccess = "true"))
	int PowerUpSpawnChance{};
};
