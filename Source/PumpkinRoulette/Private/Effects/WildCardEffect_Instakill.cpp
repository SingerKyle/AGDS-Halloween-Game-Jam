// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WildCardEffect_Instakill.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UWildCardEffect_Instakill::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "INSTAKILL");

	// will need to communicate to the gamemode or gun to change damage - probably gamemode.
	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(OwningPawn->GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sets next gunshot to have extra damage amount
		GameMode->SetNextLiveBulletWin(true);
	}
}
