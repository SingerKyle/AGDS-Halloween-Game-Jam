// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PumpkinRoulette/Public/PumpkinCardEffect.h"
#include "CardEffect_PrintToScreen.generated.h"

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UCardEffect_PrintToScreen : public UPumpkinCardEffect
{
	GENERATED_BODY()

public:

	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) override;
};
