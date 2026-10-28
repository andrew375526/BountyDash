// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectPtr.h"

#include "Floor.generated.h"

UCLASS()
class BOUNTYDASH_API AFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const class UStaticMesh* GetFloorMesh() const;
	int GetNumRepeatingMesh() const;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class USceneComponent>> FloorMeshScenes{};

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UStaticMeshComponent>> FloorMeshes{};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CollisionBox{};

	int NumRepeatingMesh{ 80 };
};
