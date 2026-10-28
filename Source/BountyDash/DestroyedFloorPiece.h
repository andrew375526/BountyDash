// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectPtr.h"

#include "DestroyedFloorPiece.generated.h"

UCLASS()
class BOUNTYDASH_API ADestroyedFloorPiece : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyedFloorPiece();
	void Death();
	void Break();

protected:
	void BeginPlay() override;

private:
	TObjectPtr<class UGeometryCollectionComponent> Destructible{};
};
