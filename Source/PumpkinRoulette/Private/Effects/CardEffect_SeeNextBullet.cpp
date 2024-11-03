// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SeeNextBullet.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

void UCardEffect_SeeNextBullet::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "See Next Bullet");
}
