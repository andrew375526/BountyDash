// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BountyDashGameMode.generated.h"

/**
 *
 */
UCLASS(MinimalAPI)
class ABountyDashGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABountyDashGameMode();
	void ChararacterScoreUp(int CharScore);

	UFUNCTION()
	double GetGameSpeed() const;

	UFUNCTION()
	double GetInverseGameSpeed() const;

	UFUNCTION()
	int GetGameLevel() const;

	void DecreaseGameSpeed();
	double GetRuntime() const;
	void PauseGame(bool pause);
	void GameOver();
	bool IsGameOver() const;

protected:
	void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess
		= "true"))
	double GameSpeed{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess
		= "true"))
	double LowestGameSpeed{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess
		= "true"))
	int GameLevel{ 1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess
		= "true"))
	int CoinsForSpeedIncrease{ 5 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess
		= "true"))
	double GameSpeedIncrease{ 50.0 };

	double Runtime{ 0.0 };
	bool bIsGameOver{ false };
	bool bIsGameOverTimerStarted{ false };
	static constexpr double TimeTilGameOver{ 2.0 };
};
