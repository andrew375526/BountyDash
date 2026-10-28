// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

#include "BountyDashCharacter.h"
#include "Obstacle.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

ACoin::ACoin()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetupAttachment(GetCollider());
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Mesh->AddLocalRotation(FRotator{ 5.0, 0.0, 0.0 });
}

void ACoin::MyOnActorBeginOverlap(AActor*, AActor* OtherActor)
{
	if (OtherActor->IsA<AObstacle>())
	{
		USphereComponent* ThisSphere{ Cast<USphereComponent>(
			GetComponentByClass(USphereComponent::StaticClass())) };

		USphereComponent* OtherSphere{ Cast<USphereComponent>(
			OtherActor->GetComponentByClass(USphereComponent::StaticClass())) };

		if (OtherSphere)
		{
			double OtherSphereDiameter{ OtherSphere->GetUnscaledSphereRadius() * 2.0 };
			AddActorLocalOffset(FVector{ 0.0, 0.0
				, OtherSphereDiameter + GetCollider()->GetUnscaledSphereRadius() });
		}
	}

	if (auto* Player{ Cast<ABountyDashCharacter>(OtherActor) })
	{
		Player->ScoreUp();
		Destroy();
	}
}
