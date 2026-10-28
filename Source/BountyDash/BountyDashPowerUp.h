#pragma once

#include "BountyDashObject.h"
#include "CoreMinimal.h"
#include "UObject/ObjectPtr.h"

#include "BountyDashPowerUp.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashPowerUp : public ABountyDashObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABountyDashPowerUp();

protected:
	void MyOnActorBeginOverlap(class AActor* OverlappedActor
		, class AActor* OtherActor) override;

	class UStaticMeshComponent* GetMesh() const;
	class UPowerUpObject* GetPowerUp() const;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh{};

	UPROPERTY()
	TObjectPtr<class UPowerUpObject> PowerUp{};
};
