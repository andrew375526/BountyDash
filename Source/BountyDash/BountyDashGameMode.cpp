// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashGameMode.h"

#include "BountyDashCharacter.h"
#include "BountyDashHUD.h"

#include "GameFramework/PlayerController.h"

ABountyDashGameMode::ABountyDashGameMode()
{
	DefaultPawnClass = ABountyDashCharacter::StaticClass();
	HUDClass = ABountyDashHUD::StaticClass();
}

void ABountyDashGameMode::ChararacterScoreUp(int CharScore)
{
	if (!CharScore) return;
	if (CharScore % CoinsForSpeedIncrease) return;
	GameSpeed += GameSpeedIncrease;
	++GameLevel;
}

double ABountyDashGameMode::GetGameSpeed() const
{
	return GameSpeed;
}

double ABountyDashGameMode::GetInverseGameSpeed() const
{
	return -GetGameSpeed();
}

int ABountyDashGameMode::GetGameLevel() const
{
	return GameLevel;
}

void ABountyDashGameMode::DecreaseGameSpeed()
{
	if ((GameSpeed - GameSpeedIncrease) < LowestGameSpeed) return;
	GameSpeed -= GameSpeedIncrease;
	--GameLevel;
}

double ABountyDashGameMode::GetRuntime() const
{
	return Runtime;
}

void ABountyDashGameMode::PauseGame(bool pause)
{
	APlayerController* PC{ GetWorld()->GetFirstPlayerController() };
	check(PC);
	if (PC) PC->SetPause(pause);
}

void ABountyDashGameMode::GameOver()
{
	bIsGameOverTimerStarted = true;
}

bool ABountyDashGameMode::IsGameOver() const
{
	return bIsGameOver;
}

void ABountyDashGameMode::Tick(float DeltaTime)
{
	if (!bIsGameOverTimerStarted) Runtime += DeltaTime;
	else
	{
		static double Time{ 0.0 };
		Time += DeltaTime;

		if (Time >= TimeTilGameOver)
		{
			bIsGameOver = true;
			Time = 0.0;
		}
	}
}
