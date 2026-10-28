// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetPointManager.h"
#include "MyTargetPoint.h"
#include "Containers/Array.h"
#include "UObject/WeakObjectPtrTemplates.h"

auto UTargetPointManager::GetTargetPoints() const
-> const TArray<TWeakObjectPtr<AMyTargetPoint>>&
{
	return TargetPoints;
}

void UTargetPointManager::AddTargetPoint(AMyTargetPoint* TP)
{
	TargetPoints.Add(TP);
	Sort();
}

void UTargetPointManager::Sort()
{
	TargetPoints.Sort(
		[](TWeakObjectPtr<AMyTargetPoint> A, TWeakObjectPtr<AMyTargetPoint> B)
		{
			// Invalid objects get pushed to the end.
			if (!A.IsValid() || !B.IsValid()) return A.IsValid() && !B.IsValid();

			return A->GetActorLocation().Y < B->GetActorLocation().Y;
		});
}
