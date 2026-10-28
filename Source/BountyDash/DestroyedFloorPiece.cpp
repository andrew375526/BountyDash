// Fill out your copyright notice in the Description page of Project Settings.

#include "DestroyedFloorPiece.h"

#include "Components/SceneComponent.h"
#include "Engine/TimerHandle.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "PhysicsEngine/PhysicsObjectBlueprintLibrary.h"

// Sets default values
ADestroyedFloorPiece::ADestroyedFloorPiece()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Destructible = CreateDefaultSubobject<UGeometryCollectionComponent>(
		TEXT("Destructible"));

	Destructible->SetupAttachment(GetRootComponent());
	Destructible->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	const ConstructorHelpers::FObjectFinder<UGeometryCollection> Mesh{ TEXT(
		"/Game/Geometry/Meshes/GC_Destroyable_Mesh.GC_Destroyable_Mesh") };

	if (Mesh.Succeeded()) Destructible->SetRestCollection(Mesh.Object);
}

void ADestroyedFloorPiece::Death()
{
	Destroy();
}

void ADestroyedFloorPiece::Break()
{
	Destructible->SetSimulatePhysics(true);

	const FVector Origin{
		GetActorLocation().X + 200,
		GetActorLocation().Y + FMath::RandRange(-10, 10),
		GetActorLocation().Z + FMath::RandRange(-10, 10), };

	UPhysicsObjectBlueprintLibrary::ApplyRadialImpulse(
		Destructible,
		Origin,
		1000.0f,
		20000.0f,
		ERadialImpulseFalloff::RIF_Linear,
		true,
		100.0f,
		true,
		0.0f,
		1.0f
	);

	Destructible->SetSimulatePhysics(false);
}

void ADestroyedFloorPiece::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Timer{};

	GetWorld()->GetTimerManager().SetTimer(
		Timer, this, &ADestroyedFloorPiece::Death, 4.0f, false);
}
