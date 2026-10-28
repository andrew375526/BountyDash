// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectPtr.h"
#include "Traits/IsTSubclassOf.h"

#include "ObstacleSpawner.generated.h"

UCLASS()
class BOUNTYDASH_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacleSpawner();

protected:
	void SpawnObstacle();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly
		, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AObstacle>> ObstaclesToSpawn{};

	UPROPERTY()
	TObjectPtr<class USceneComponent> Scene{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly
		, meta = (AllowPrivateAccess = "true"))
	double SpawnTimer{ 1.5 };

	double TimeSinceLastSpawn{};
};
