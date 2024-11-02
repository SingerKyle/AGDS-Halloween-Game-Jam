// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PumpkinCardData.generated.h"

class UPumpkinCardEffect;
/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UPumpkinCardData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CardData")
	UMaterialInstance* Material;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "CardData")
	TArray<TObjectPtr<UPumpkinCardEffect>> Effects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CardData")
	bool bIsWildcard = false;
};
