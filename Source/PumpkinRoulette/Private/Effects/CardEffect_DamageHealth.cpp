// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_DamageHealth.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_DamageHealth::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "+1 Damage to next shot");

	// will need to communicate to the gamemode or gun to change damage - probably gamemode.
	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sets next gunshot to have extra damage amount
		GameMode->SetDamageModifier(DamageAmount);
	}

}