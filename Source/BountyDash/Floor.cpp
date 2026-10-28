// Fill out your copyright notice in the Description page of Project Settings.

#include "Floor.h"

#include "BountyDashGameMode.h"
#include "DestroyedFloorPiece.h"
#include "GameModeUtils.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Containers/UnrealString.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ObjectPtr.h"
#include "UObject/NameTypes.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "ActorInitDataUtils.h"

namespace
{
	int WrapAround(int index, int length)
	{
		const double Quotient{
			FMath::Floor(static_cast<double>(index) / length) };

		return index - Quotient * length;
	}
}

// Sets default values
AFloor::AFloor()
{
	// Set this actor to call Tick() every frame. 
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMesh{ TEXT(
		"/Game/Geometry/Meshes/Destroyable_Mesh.Destroyable_Mesh") };

	ConstructorHelpers::FObjectFinder<UMaterial> FloorMaterial{ TEXT(
		"/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles") };

	if (FloorMesh.Succeeded())
	{
		double ScenePosition{ -ActorInitDataUtils::GetXScenePositionFromOrigin(
			*(FloorMesh.Object), GetNumRepeatingMesh()) };

		for (int i{ 0 }; i < GetNumRepeatingMesh(); ++i)
		{
			USceneComponent* Scene{ CreateDefaultSubobject<USceneComponent>(
				FName{ TEXT("Scene") + FString::FromInt(i) }) };

			Scene->SetupAttachment(RootComponent);
			Scene->SetRelativeLocation(FVector{ ScenePosition, 0.0, 0.0 });
			FloorMeshScenes.Add(Scene);

			ScenePosition
				+= ActorInitDataUtils::GetMeshXBounds(*FloorMesh.Object);

			UStaticMeshComponent* Mesh{
				CreateDefaultSubobject<UStaticMeshComponent>(
					FName{ TEXT("Mesh") + FString::FromInt(i) }) };

			Mesh->SetupAttachment(Scene);
			Mesh->SetRelativeLocation(FVector{ 0.0, 0.0, 0.0 });
			Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

			if (FloorMaterial.Succeeded())
			{
				Mesh->SetStaticMesh(FloorMesh.Object);
				Mesh->SetMaterial(0, FloorMaterial.Object);
			}

			FloorMeshes.Add(Mesh);
		}

		ActorInitDataUtils::Position::SetKillPoint(*this);
		ActorInitDataUtils::Position::SetSpawnPoint(*this);

		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT(
			"CollisionBox"));

		CollisionBox->SetupAttachment(RootComponent);
		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));

		CollisionBox->SetBoxExtent(FVector{
			ActorInitDataUtils::Position::GetSpawnPoint(),
			FloorMesh.Object->GetBounds().BoxExtent.Y,
			FloorMesh.Object->GetBounds().BoxExtent.Z });
	}
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i{ 0 }; i < FloorMeshScenes.Num(); ++i)
	{
		using GameModeUtils::GetCustomGameMode;

		const auto CustomGameMode{ GetCustomGameMode<ABountyDashGameMode>(
			*GetWorld()) };

		const double XOffset{
			CustomGameMode->GetInverseGameSpeed() * DeltaTime };

		FloorMeshScenes[i]->AddLocalOffset(FVector{
			XOffset,
			0.0,
			0.0 });

		if (FloorMeshScenes[i]->GetComponentLocation().X
			<= ActorInitDataUtils::Position::GetKillPoint())
		{
			//auto* Piece{ GetWorld()->SpawnActor<ADestroyedFloorPiece>(
			//	ADestroyedFloorPiece::StaticClass(),
			//	FloorMeshScenes[i]->GetComponentTransform()) };

			//check(Piece);
			//if (Piece) Piece->Break();
			const int SceneBefore{ WrapAround(i - 1, FloorMeshScenes.Num()) };

			const double SpawnPoint{
				FloorMeshScenes[SceneBefore]->GetComponentLocation().X
				+ FloorMeshes[SceneBefore]->GetBounds().BoxExtent.X
				+ FloorMeshes[i]->GetBounds().BoxExtent.X };

			const bool IsLastComponent{
				SceneBefore == FloorMeshScenes.Num() - 1 };

			FloorMeshScenes[i]->SetRelativeLocation(FVector{
				IsLastComponent ? SpawnPoint + XOffset : SpawnPoint,
				0.0,
				0.0 });
		}
	}
}

const UStaticMesh* AFloor::GetFloorMesh() const
{
	return FloorMeshes.Top()->GetStaticMesh();
}

int AFloor::GetNumRepeatingMesh() const
{
	return NumRepeatingMesh;
}
