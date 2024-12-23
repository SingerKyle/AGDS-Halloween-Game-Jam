// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PumpkinRoulette/Public/PumpkinCardEffect.h"
#include "CardEffect_DamageHealth.generated.h"

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UCardEffect_DamageHealth : public UPumpkinCardEffect
{
	GENERATED_BODY()
	
	int DamageAmount = -1;
public:
	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) override;
};
