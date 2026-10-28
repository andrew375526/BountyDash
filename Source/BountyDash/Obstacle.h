// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDashObject.h"
#include "CoreMinimal.h"
#include "UObject/ObjectPtr.h"

#include "Obstacle.generated.h"

UCLASS(BlueprintType)
class BOUNTYDASH_API AObstacle : public ABountyDashObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacle();

protected:
	class UGeometryCollectionComponent* GetGeometryCollection() const;

	void MyOnActorBeginOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor) override;

	void MyOnActorEndOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGeometryCollectionComponent> Mesh{};

	bool bIsFirstContact{ true };
};
