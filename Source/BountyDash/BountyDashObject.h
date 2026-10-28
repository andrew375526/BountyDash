// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BountyDashObject.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABountyDashObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void MyOnActorBeginOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor);

	UFUNCTION()
	virtual void MyOnActorEndOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor);

	class USphereComponent* GetCollider() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Collider{};
};
