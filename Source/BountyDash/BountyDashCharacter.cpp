// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashCharacter.h"

#include "ActorInitDataUtils.h"
#include "BountyDashGameMode.h"
#include "Coin.h"
#include "CoinManager.h"
#include "GameModeUtils.h"
#include "MyTargetPoint.h"

#include "PowerUpObject.h"

#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EngineUtils.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/TargetPoint.h"
#include "Engine/TimerHandle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "UObject/WeakObjectPtrTemplates.h"

namespace
{
	bool IsValidLocation(const UWorld& World, int Location)
	{
		return ActorInitDataUtils::GetTargetPoints(World).IsValidIndex(
			Location);
	}
}

// Sets default values
ABountyDashCharacter::ABountyDashCharacter()
{
	// Set this character to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> MyAnimBP{ TEXT(
		"/Game/Barrel_Hopper/Blueprints"
		"/BH_Character_AnimBP.BH_Character_AnimBP") };

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MyMesh{ TEXT(
		"/Game/Barrel_Hopper/Character/Meshes"
		"/SKM_Manny_Simple.SKM_Manny_Simple") };

	if (MyAnimBP.Succeeded() && MyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MyMesh.Object);
		GetMesh()->SetAnimInstanceClass(MyAnimBP.Object->GeneratedClass);
	}

	GetMesh()->SetRelativeLocation(FVector{ 0.0, 0.0
		, -(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()) });

	GetMesh()->SetRelativeRotation(FRotator{ 0.0, -90.0, 0.0 });
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT(
		"CameraBoom"));

	CameraBoom->AttachToComponent(RootComponent
		, FAttachmentTransformRules::KeepRelativeTransform);

	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->AddRelativeLocation(FVector{ 0.0, 0.0, 160.0 });

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT(
		"FollowCamera"));

	FollowCamera->AttachToComponent(CameraBoom
		, FAttachmentTransformRules::KeepRelativeTransform
		, USpringArmComponent::SocketName);

	FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0, 0.0, 0.0)));

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this
		, &ABountyDashCharacter::MyOnComponentBeginOverlap);

	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this
		, &ABountyDashCharacter::MyOnComponentEndOverlap);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	HitObstacleSound
		= CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));

	HitObstacleSound->bAutoActivate = false;
	HitObstacleSound->SetupAttachment(GetRootComponent());

	const ConstructorHelpers::FObjectFinder<USoundCue> HitSoundCue{
		TEXT("/Game/GruntCue.GruntCue") };

	if (HitSoundCue.Succeeded())
		HitObstacleSound->SetSound(HitSoundCue.Object);

	DingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Ding"));
	DingSound->bAutoActivate = false;
	DingSound->SetupAttachment(GetRootComponent());

	const ConstructorHelpers::FObjectFinder<USoundCue> myCue(
		TEXT("/Game/DingCue.DingCue"));

	if (myCue.Succeeded()) DingSound->SetSound(myCue.Object);
}

// Called when the game starts or when spawned
void ABountyDashCharacter::BeginPlay()
{
	Super::BeginPlay();

	int PossibleLocations{
		ActorInitDataUtils::GetTargetPoints(*GetWorld()).Num() };

	// Assuming locations are in sorted order.
	int Center{ PossibleLocations / 2 };

	auto bIsOdd{ static_cast<bool>(PossibleLocations % 2) };
	CurrentLocation = bIsOdd ? Center : Center - 1;
}

// Called every frame
void ABountyDashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	using ActorInitDataUtils::GetTargetPoints;

	if (!(GetTargetPoints(*GetWorld()).IsEmpty()))
	{
		FVector TargetLocation{ GetTargetPoints(*GetWorld())[CurrentLocation]
			->GetActorLocation() };

		TargetLocation.Z = GetActorLocation().Z;
		TargetLocation.X = GetActorLocation().X;

		if (TargetLocation != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLocation
				, CharSpeed * DeltaTime));
		}
	}

	if (bIsBeingPushed)
	{
		using GameModeUtils::GetCustomGameMode;

		const auto* CustomGameMode{
			GetCustomGameMode<ABountyDashGameMode>(*GetWorld()) };

		AddActorLocalOffset(FVector{
			CustomGameMode->GetInverseGameSpeed() * DeltaTime,
			0.0,
			0.0 });
	}

	if (bCanMagnet)
	{
		for (TWeakObjectPtr<ACoin> Coin
			: GetWorld()->GetSubsystem<UCoinManager>()->GetCoins())
		{
			if (!Coin.IsValid()) continue;
			const FVector Between{ GetActorLocation() - Coin->GetActorLocation() };

			if (Between.Size() <= MagnetReach)
			{
				const FVector Lerp{ FMath::VInterpTo(
					Coin->GetActorLocation(),
					GetActorLocation(),
					DeltaTime,
					5.0f) };

				Coin->SetActorLocation(Lerp);
			}
		}
	}

	if (GetActorLocation().X < ActorInitDataUtils::Position::GetKillPoint())
	{
		using GameModeUtils::GetCustomGameMode;

		auto* CustomGameMode{
			GetCustomGameMode<ABountyDashGameMode>(*GetWorld()) };

		CustomGameMode->GameOver();
	}
}

// Called to bind functionality to input
void ABountyDashCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this
		, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this
		, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("MoveRight"), IE_Pressed, this,
		&ABountyDashCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("MoveLeft"), IE_Pressed, this,
		&ABountyDashCharacter::MoveLeft);

	PlayerInputComponent->BindAction(TEXT("Reset"), IE_Pressed, this,
		&ABountyDashCharacter::Reset).bExecuteWhenPaused = true;
}

void ABountyDashCharacter::MoveRight()
{
	if (!Controller) return;
	if (!IsValidLocation(*GetWorld(), CurrentLocation + 1)) return;
	++CurrentLocation;
}

void ABountyDashCharacter::MoveLeft()
{
	if (!Controller) return;
	if (!IsValidLocation(*GetWorld(), CurrentLocation - 1)) return;
	--CurrentLocation;
}

void ABountyDashCharacter::MyOnComponentBeginOverlap(
	UPrimitiveComponent*, AActor* OtherActor
	, UPrimitiveComponent*, int32
	, bool, const struct FHitResult&)
{
}

void ABountyDashCharacter::MyOnComponentEndOverlap(
	UPrimitiveComponent*, AActor* OtherActor
	, UPrimitiveComponent*, int32)
{
}

void ABountyDashCharacter::StopSmash()
{
	bCanSmash = false;
}

void ABountyDashCharacter::StopMagnet()
{
	bCanMagnet = false;
}

void ABountyDashCharacter::Reset()
{
	check(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BountyDashMap"));
}

void ABountyDashCharacter::ScoreUp()
{
	check(DingSound);
	if (DingSound) DingSound->Play();
	++Score;

	GameModeUtils::GetCustomGameMode<ABountyDashGameMode>(*GetWorld())
		->ChararacterScoreUp(Score);
}

void ABountyDashCharacter::PowerUp(EPowerUp Type)
{
	switch (Type)
	{
		using enum EPowerUp;
	case Speed:
		GameModeUtils::GetCustomGameMode<ABountyDashGameMode>(*GetWorld())
			->DecreaseGameSpeed();

		break;
	case Smash:
	{
		bCanSmash = true;
		FTimerHandle Timer{};

		GetWorld()->GetTimerManager().SetTimer(
			Timer, this, &ABountyDashCharacter::StopSmash, SmashTime, false);

		break;
	}
	case Magnet:
	{
		bCanMagnet = true;
		FTimerHandle Timer{};

		GetWorld()->GetTimerManager().SetTimer(
			Timer, this, &ABountyDashCharacter::StopMagnet, MagnetTime, false);

		break;
	}
#if defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_DEBUG)
	default:
		UE_LOG(LogTemp, Warning, TEXT("EnumType: %d"), static_cast<int>(Type));
		check(!"Unhandled case.");
	}
#endif
}

bool ABountyDashCharacter::CanSmash() const
{
	return bCanSmash;
}

int ABountyDashCharacter::GetScore() const
{
	return Score;
}

void ABountyDashCharacter::PlayHitSound()
{
	check(HitObstacleSound);
	if (HitObstacleSound) HitObstacleSound->Play();
}
