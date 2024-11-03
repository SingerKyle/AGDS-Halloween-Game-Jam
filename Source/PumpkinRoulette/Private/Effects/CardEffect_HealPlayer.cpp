// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_HealPlayer.h"
#include "PumpkinHealthComponent.h"

void UCardEffect_HealPlayer::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	
	if (OwningPawn)
	{
		UPumpkinHealthComponent* HealthComponent = Cast<UPumpkinHealthComponent>(OwningPawn->GetComponentByClass<UPumpkinHealthComponent>());
		if (HealthComponent)
		{
			HealthComponent->AdjustHealth(1);
			GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "+1 Healing to player");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "Can't find component");
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "OwningPawn isn't valid");
	}
	
}