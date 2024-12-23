// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PumpkinCardEffect.h"
#include "WildCardEffect_Instakill.generated.h"

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UWildCardEffect_Instakill : public UPumpkinCardEffect
{
	GENERATED_BODY()

public:
	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) override;
};
