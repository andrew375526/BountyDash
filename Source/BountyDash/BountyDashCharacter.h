// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "GameFramework/Character.h"
#include "BountyDashCharacter.generated.h"

enum class EPowerUp : uint8;

UCLASS()
class BOUNTYDASH_API ABountyDashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABountyDashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight();
	void MoveLeft();

	UFUNCTION()
	void MyOnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor, class UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep
		, const struct FHitResult& SweepResult);

	UFUNCTION()
	void MyOnComponentEndOverlap(
		class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void StopSmash();

	UFUNCTION()
	void StopMagnet();

	void Reset();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;

	void ScoreUp();
	void PowerUp(EPowerUp);
	bool bIsBeingPushed{ false };
	bool CanSmash() const;
	int GetScore() const;
	void PlayHitSound();

private:
	UPROPERTY(VisibleAnywhere, Category = "Logic")
	double CharSpeed{ 10.0 };

	UPROPERTY(VisibleAnywhere, Category = "Logic")
	TObjectPtr<class UAudioComponent> HitObstacleSound{};

	UPROPERTY(VisibleAnywhere, Category = "Logic")
	TObjectPtr<class UAudioComponent> DingSound{};

	UPROPERTY(BlueprintReadOnly, Category = "Camera", meta = (
		AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom{};

	UPROPERTY(BlueprintReadOnly, Category = "Camera", meta = (
		AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera{};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Score{ 0 };

	UPROPERTY(
		EditAnywhere,
		Category = "PowerUps",
		meta = (AllowPrivateAccess = "true"))
	double SmashTime{};

	UPROPERTY(
		EditAnywhere,
		Category = "PowerUps",
		meta = (AllowPrivateAccess = "true"))
	double MagnetTime{};

	UPROPERTY(
		EditAnywhere,
		Category = "PowerUps",
		meta = (AllowPrivateAccess = "true"))
	double MagnetReach;

	bool bCanSmash{ false };
	bool bCanMagnet{ false };
	FVector DesiredLocation{};
	int CurrentLocation{};
};
