// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_Jumpscare.h"

void UCardEffect_Jumpscare::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "JUMPSCARE");
}
