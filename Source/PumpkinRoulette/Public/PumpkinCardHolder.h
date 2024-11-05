// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PumpkinCardHolder.generated.h"

class APumpkinCard;
class UArrowComponent;

UENUM(BlueprintType)
enum class ECardHolderType : uint8
{
	Player1,
	Player2
};

UCLASS()
class PUMPKINROULETTE_API APumpkinCardHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APumpkinCardHolder();

	UFUNCTION()
	void OnCardPlayed();

	bool HasCard() const;
	
	void SetCard(APumpkinCard* NewCard);

public:
	
	UPROPERTY(EditAnywhere, Category = "Components")
	ECardHolderType CardHolderType;

	UPROPERTY(EditAnywhere, Category = "Components")
	int32 CardSlotIndex;

	UPROPERTY()
	APumpkinCard* Card;
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> ArrowComponent;
};
