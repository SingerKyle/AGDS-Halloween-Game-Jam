// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_HealPlayer.h"
#include "PumpkinHealthComponent.h"
#include "PumpkinPlayerInterface.h"

void UCardEffect_HealPlayer::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);
	
	if (OwningPawn)
	{
		if (UPumpkinHealthComponent* HealthComponent = Cast<UPumpkinHealthComponent>(OwningPawn->GetComponentByClass<UPumpkinHealthComponent>()))
		{
			HealthComponent->AdjustHealth(1);
			IPumpkinPlayerInterface::Execute_DisplayNotification(TargetPawn, FString("+1 Damage"));
		}
	}
}
