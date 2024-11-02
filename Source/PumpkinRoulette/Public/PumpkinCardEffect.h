// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PumpkinCardEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, Blueprintable, BlueprintType, EditInlineNew)
class PUMPKINROULETTE_API UPumpkinCardEffect : public UObject
{
	GENERATED_BODY()

public:
	
	virtual void Execute(APawn* OwningPawn, APawn* TargetPawn) {};
};
