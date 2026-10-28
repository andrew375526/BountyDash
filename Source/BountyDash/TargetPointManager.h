// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "TargetPointManager.generated.h"

/**
 *
 */
UCLASS()
class BOUNTYDASH_API UTargetPointManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	auto GetTargetPoints() const 
		-> const TArray<TWeakObjectPtr<class AMyTargetPoint>>&;
	
	void AddTargetPoint(class AMyTargetPoint* TP);

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<class AMyTargetPoint>> TargetPoints;

	void Sort();
};
