// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_DamageHealth.h"

void UCardEffect_DamageHealth::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "+1 Damage to next shot");

	// will need to communicate to the gun
}