// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_Jumpscare.h"

#include "PumpkinPlayerInterface.h"

void UCardEffect_Jumpscare::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	IPumpkinPlayerInterface::Execute_DoJumpscare(TargetPawn);

}
