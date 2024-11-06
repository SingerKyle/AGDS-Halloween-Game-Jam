// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_StealCard.h"

#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_StealCard::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	APumpkinGameModeBase* GameMode = OwningPawn->GetWorld()->GetAuthGameMode<APumpkinGameModeBase>();

	GameMode->StealCard(TargetPawn);
}
