// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "BountyDashCharacter.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsEngine/PhysicsObjectBlueprintLibrary.h"

namespace
{
	void OnPlayerCollision(
		const AObstacle& Obstacle,
		ABountyDashCharacter& Player,
		UGeometryCollectionComponent& GeometryCollection)
	{
		const FVector Between{
				Obstacle.GetActorLocation() - Player.GetActorLocation() };

		const double DP{ FVector::DotProduct(
			Between.GetSafeNormal(), Player.GetActorForwardVector()) };

		const double AlignmentAngle{
			FMath::RadiansToDegrees(FMath::Acos(DP)) };

		if (AlignmentAngle < 60.0)
		{
			if (Player.CanSmash())
			{
				GeometryCollection.SetSimulatePhysics(true);

				const FVector Origin{
					Obstacle.GetActorLocation().X + 70,
					Player.GetActorLocation().Y,
					Obstacle.GetActorLocation().Z + 40 };

				UPhysicsObjectBlueprintLibrary::ApplyRadialImpulse(
					&GeometryCollection,
					Origin,
					660.0f,
					40000.0f,
					ERadialImpulseFalloff::RIF_Linear,
					true,
					50.0f,
					true,
					0.0f,
					1.0f
				);

				GeometryCollection.SetSimulatePhysics(false);
			}
			else Player.bIsBeingPushed = true;
		}
	}
}

// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame. 
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetCollider());
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

UGeometryCollectionComponent* AObstacle::GetGeometryCollection() const
{
	return Mesh;
}

void AObstacle::MyOnActorBeginOverlap(AActor*, AActor* OtherActor)
{
	check(GetGeometryCollection());

	if (auto* Player{ Cast<ABountyDashCharacter>(OtherActor) })
	{
		if (bIsFirstContact) Player->PlayHitSound();
		bIsFirstContact = false;
		OnPlayerCollision(*this, *Player, *GetGeometryCollection());
	}
}

void AObstacle::MyOnActorEndOverlap(AActor*, AActor* OtherActor)
{
	if (auto* Player{ Cast<ABountyDashCharacter>(OtherActor) })
		Player->bIsBeingPushed = false;
}
