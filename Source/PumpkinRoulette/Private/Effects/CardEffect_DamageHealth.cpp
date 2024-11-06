// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_DamageHealth.h"

#include "PumpkinPlayerInterface.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_DamageHealth::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);
	
	// will need to communicate to the gamemode or gun to change damage - probably gamemode.
	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(OwningPawn->GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sets next gunshot to have extra damage amount
		GameMode->SetDamageModifier(DamageAmount);
		IPumpkinPlayerInterface::Execute_DisplayNotification(TargetPawn, FString("+1 Damage"));
	}

}