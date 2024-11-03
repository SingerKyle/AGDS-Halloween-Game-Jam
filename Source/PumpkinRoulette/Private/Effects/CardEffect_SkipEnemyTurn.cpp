// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SkipEnemyTurn.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_SkipEnemyTurn::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sets next gunshot to have extra damage amount
		GameMode->SetSkipNextTurn(true);
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "Skip Enemy Turn");

	}
}
