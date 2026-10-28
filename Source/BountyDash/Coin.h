// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDashObject.h"
#include "CoreMinimal.h"
#include "UObject/ObjectPtr.h"

#include "Coin.generated.h"

/**
 *
 */
UCLASS()
class BOUNTYDASH_API ACoin : public ABountyDashObject
{
	GENERATED_BODY()

public:
	ACoin();

protected:
	void Tick(float DeltaTime) override;

	void MyOnActorBeginOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh{};
};
