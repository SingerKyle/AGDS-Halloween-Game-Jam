// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/PumpkinCard.h"

#include "Net/UnrealNetwork.h"
#include "PumpkinRoulette/Public/PumpkinCardData.h"
#include "PumpkinRoulette/Public/PumpkinCardEffect.h"

// Sets default values
APumpkinCard::APumpkinCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
}

void APumpkinCard::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APumpkinCard, CardData);
}

// Called when the game starts or when spawned
void APumpkinCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APumpkinCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APumpkinCard::SetCardData(UPumpkinCardData* NewCardData)
{
	CardData = NewCardData;
	OnRep_CardData();
}

void APumpkinCard::PlayCard(APawn* PawnInstigator, APawn* Target)
{
	ServerPlayCard(PawnInstigator, Target);
}

void APumpkinCard::ServerPlayCard_Implementation(APawn* PawnInstigator, APawn* Target)
{
	// Play the card server side,
	// @NOTE (Denis): We'll need to figure out exactly how a card is played
	for (const auto Effect : CardData->Effects)
	{
		if (Effect)
		{
			Effect->Execute(PawnInstigator, Target);
		}
	}
}

void APumpkinCard::OnRep_CardData()
{
	CardMesh->SetMaterial(0, CardData->Material);
}
