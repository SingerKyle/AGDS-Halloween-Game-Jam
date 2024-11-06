// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinCardHolder.h"

#include "PumpkinCard.h"
#include "Components/ArrowComponent.h"

// Sets default values
APumpkinCardHolder::APumpkinCardHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	ArrowComponent->SetupAttachment(GetRootComponent());
	
	CardHolderType = ECardHolderType::Player1;
	CardSlotIndex = 0;
}

void APumpkinCardHolder::OnCardPlayed()
{
	Card = nullptr;
}

bool APumpkinCardHolder::HasCard() const
{
	return Card != nullptr;
}

void APumpkinCardHolder::SetCard(APumpkinCard* NewCard)
{
	Card = NewCard;
	if (Card)
	{
		Card->OnCardPlayed.AddDynamic(this, &ThisClass::OnCardPlayed);
	}
}
