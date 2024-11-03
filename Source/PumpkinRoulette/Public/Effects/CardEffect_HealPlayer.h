// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PumpkinCardData.h"
#include "PumpkinRoulette/Public/PumpkinCardEffect.h"
#include "CardEffect_HealPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UCardEffect_HealPlayer : public UPumpkinCardEffect
{
	GENERATED_BODY()
	
public:

	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) override;
};
