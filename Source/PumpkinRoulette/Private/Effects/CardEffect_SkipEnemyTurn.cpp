// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SkipEnemyTurn.h"

#include "PumpkinPlayerInterface.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_SkipEnemyTurn::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(OwningPawn->GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sets next gunshot to have extra damage amount
		GameMode->SetSkipNextTurn(true);
		IPumpkinPlayerInterface::Execute_DisplayNotification(TargetPawn, FString("Skips your next turn"));
	}
}
