// Fill out your copyright notice in the Description page of Project Settings.

#include "ObstacleSpawner.h"
#include "ActorInitDataUtils.h"
#include "BountyDashGameMode.h"
#include "GameModeUtils.h"
#include "MyTargetPoint.h"
#include "Obstacle.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AObstacleSpawner::AObstacleSpawner()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;
	TimeSinceLastSpawn = SpawnTimer;
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastSpawn += DeltaTime;

	const int GameLevel{ GameModeUtils::GetCustomGameMode<ABountyDashGameMode>(
		*GetWorld())->GetGameLevel() };

	const double TrueSpawnTime{ SpawnTimer / GameLevel };

	if (TimeSinceLastSpawn > TrueSpawnTime)
	{
		SpawnObstacle();
		TimeSinceLastSpawn = 0.0;
	}
}

void AObstacleSpawner::SpawnObstacle()
{
	using ActorInitDataUtils::GetTargetPoints;

	if (GetTargetPoints(*GetWorld()).IsEmpty() || ObstaclesToSpawn.IsEmpty())
		return;

	const int32 Spawner{ FMath::Rand() % GetTargetPoints(*GetWorld()).Num() };
	const int32 Obstacle{ FMath::Rand() % ObstaclesToSpawn.Num() };

	FTransform ObstacleTransform{ GetTargetPoints(*GetWorld())[Spawner]
		->GetTransform() };

	ObstacleTransform.SetLocation(FVector{
		ActorInitDataUtils::Position::GetSpawnPoint(),
		ObstacleTransform.GetLocation().Y,
		ObstacleTransform.GetLocation().Z });

	AObstacle* SpawnedObstacle{ GetWorld()->SpawnActor<AObstacle>(
		ObstaclesToSpawn[Obstacle], ObstacleTransform
		, FActorSpawnParameters{}) };

	if (!SpawnedObstacle) return;

	USphereComponent* Sphere{ Cast<USphereComponent>(
		SpawnedObstacle->GetComponentByClass(
			USphereComponent::StaticClass())) };

	if (!Sphere) return;

	SpawnedObstacle->AddActorLocalOffset(
		FVector{ 0.0, 0.0, Sphere->GetUnscaledSphereRadius() });
}
