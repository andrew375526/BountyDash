// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashObject.h"
#include "ActorInitDataUtils.h"
#include "BountyDashGameMode.h"
#include "GameModeUtils.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ABountyDashObject::ABountyDashObject()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	OnActorBeginOverlap.AddDynamic(
		this, &ABountyDashObject::MyOnActorBeginOverlap);

	OnActorEndOverlap.AddDynamic(
		this, &ABountyDashObject::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void ABountyDashObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABountyDashObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const double InverseGameSpeed{
		GameModeUtils::GetCustomGameMode<ABountyDashGameMode>(
			*GetWorld())->GetInverseGameSpeed() };

	AddActorLocalOffset(FVector{ InverseGameSpeed * DeltaTime, 0.0, 0.0 });

	if (GetActorLocation().X <= ActorInitDataUtils::Position::GetKillPoint())
		Destroy();
}

void ABountyDashObject::MyOnActorBeginOverlap(AActor* OverlappedActor
	, AActor* OtherActor)
{

}

void ABountyDashObject::MyOnActorEndOverlap(AActor* OverlappedActor
	, AActor* OtherActor)
{

}

USphereComponent* ABountyDashObject::GetCollider() const
{
	return Collider;
}
