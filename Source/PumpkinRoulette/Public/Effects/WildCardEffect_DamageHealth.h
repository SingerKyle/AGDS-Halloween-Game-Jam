// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PumpkinCardEffect.h"
#include "WildCardEffect_DamageHealth.generated.h"

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UWildCardEffect_DamageHealth : public UPumpkinCardEffect
{
	GENERATED_BODY()
	
	int DamageAmount = -3;
	int HealAmount = 3;
public:
	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) override;
};
