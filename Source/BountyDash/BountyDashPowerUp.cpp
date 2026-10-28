#include "BountyDashPowerUp.h"
#include "BountyDashCharacter.h"
#include "Obstacle.h"
#include "PowerUpObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABountyDashPowerUp::ABountyDashPowerUp()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PowerUp = CreateDefaultSubobject<UPowerUpObject>(TEXT("PowerUp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetupAttachment(GetRootComponent());
	FStringView AssetName{};

	switch (PowerUp->GetType())
	{
		using enum EPowerUp;
	case Speed:
		AssetName = TEXT("/Game/StarterContent/Shapes"
			"/Shape_QuadPyramid.Shape_QuadPyramid");

		break;
	case Smash:
		AssetName = TEXT("/Game/StarterContent/Shapes"
			"/Shape_WideCapsule.Shape_WideCapsule");

		break;
	case Magnet:
		AssetName = TEXT("/Game/StarterContent/Shapes"
			"/Shape_Pipe_180.Shape_Pipe_180");

		break;
#ifdef UE_BUILD_DEVELOPMENT
	default:
		check(!"Missing case.");
#endif
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh{
		AssetName.GetData() };

	if (FoundMesh.Succeeded())
	{
		Mesh->SetStaticMesh(FoundMesh.Object);
	}
}

void ABountyDashPowerUp::MyOnActorBeginOverlap(AActor*, AActor* OtherActor)
{
	if (OtherActor->IsA<AObstacle>())
	{
		USphereComponent* ThisSphere{ Cast<USphereComponent>(
			GetComponentByClass(USphereComponent::StaticClass())) };

		USphereComponent* OtherSphere{ Cast<USphereComponent>(
			OtherActor->GetComponentByClass(USphereComponent::StaticClass())) };

		if (OtherSphere)
		{
			double SphereDiameter{ GetCollider()->GetUnscaledSphereRadius() * 2.0 };
			AddActorLocalOffset(FVector{ 0.0, 0.0
				, GetCollider()->GetUnscaledSphereRadius() + SphereDiameter });
		}
	}

	if (auto* Player{ Cast<ABountyDashCharacter>(OtherActor) })
	{
		Player->PowerUp(GetPowerUp()->GetType());
		Destroy();
	}
}

UStaticMeshComponent* ABountyDashPowerUp::GetMesh() const
{
	return Mesh;
}

UPowerUpObject* ABountyDashPowerUp::GetPowerUp() const
{
	return PowerUp;
}
