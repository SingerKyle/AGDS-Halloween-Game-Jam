// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinRoulette/Public/Effects/CardEffect_PrintToScreen.h"

void UCardEffect_PrintToScreen::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "Hello World!");
}