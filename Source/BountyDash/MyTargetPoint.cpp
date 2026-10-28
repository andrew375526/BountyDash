// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTargetPoint.h"
#include "TargetPointManager.h"

void AMyTargetPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UTargetPointManager* TPM{
		GetWorld()->GetSubsystem<UTargetPointManager>() };

	TPM->AddTargetPoint(this);
}
