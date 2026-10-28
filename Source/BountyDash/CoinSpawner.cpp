// Fill out your copyright notice in the Description page of Project Settings.

#include "CoinSpawner.h"

#include "ActorInitDataUtils.h"
#include "BountyDashPowerUp.h"
#include "Coin.h"
#include "CoinManager.h"
#include "MyTargetPoint.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

// Sets default values
ACoinSpawner::ACoinSpawner()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ACoinSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CoinSetTimerHandle, this
		, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);

	GetWorldTimerManager().SetTimer(SpawnerMoveTimerHandle, this
		, &ACoinSpawner::MoveSpawner, SpawnerMovementTimeInterval, true);
}

void ACoinSpawner::SpawnCoinSet()
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);
	GetWorldTimerManager().ClearTimer(CoinSetTimerHandle);

	GetWorldTimerManager().SetTimer(CoinTimerHandle, this
		, &ACoinSpawner::SpawnCoin, CoinTimeInterval, true);
}

void ACoinSpawner::SpawnPowerUp()
{
	FTransform Transform{ ActorInitDataUtils::GetTargetPoints(*GetWorld())
		[TargetLoc]->GetTransform() };

	Transform.SetLocation(FVector{
		ActorInitDataUtils::Position::GetSpawnPoint(),
		Transform.GetLocation().Y,
		Transform.GetLocation().Z });

	auto* PowerUp{ GetWorld()->SpawnActor<ABountyDashPowerUp>(
		PowerUpObject, Transform, FActorSpawnParameters{}) };

	if (PowerUp)
	{
		USphereComponent* PowerUpSphereComponent{ Cast<USphereComponent>(
		PowerUp->GetComponentByClass(USphereComponent::StaticClass())) };

		if (PowerUpSphereComponent)
		{
			PowerUp->AddActorLocalOffset(FVector{
				0.0, 0.0, PowerUpSphereComponent->GetUnscaledSphereRadius() });
		}

	}
}

void ACoinSpawner::MoveSpawner()
{
	TargetLoc = FMath::Rand()
		% ActorInitDataUtils::GetTargetPoints(*GetWorld()).Num();
}

void ACoinSpawner::SpawnCoin()
{
	if ((FMath::Rand() % 100) < PowerUpSpawnChance)
	{
		SpawnPowerUp();
		--NumCoinsToSpawn;
		return;
	}

	FTransform CoinTransform{ ActorInitDataUtils::GetTargetPoints(*GetWorld())
		[TargetLoc]->GetTransform() };

	CoinTransform.SetLocation(FVector{
		ActorInitDataUtils::Position::GetSpawnPoint(),
		CoinTransform.GetLocation().Y,
		CoinTransform.GetLocation().Z });

	ACoin* Coin{ GetWorld()->SpawnActor<ACoin>(CoinObject, CoinTransform
		, FActorSpawnParameters{}) };

	GetWorld()->GetSubsystem<UCoinManager>()->AddCoin(Coin);

	if (!Coin) return;

	USphereComponent* CoinSphereComponent{ Cast<USphereComponent>(
		Coin->GetComponentByClass(USphereComponent::StaticClass())) };

	if (!CoinSphereComponent) return;

	Coin->AddActorLocalOffset(FVector{ 0.0, 0.0
		, CoinSphereComponent->GetUnscaledSphereRadius() });

	--NumCoinsToSpawn;
	if (NumCoinsToSpawn) return;

	GetWorldTimerManager().SetTimer(CoinSetTimerHandle, this
		, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);

	GetWorldTimerManager().ClearTimer(CoinTimerHandle);
}
