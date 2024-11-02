// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameModeBase.h"

#include "PumpkinPlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "PumpkinHealthComponent.h"

APumpkinGameModeBase::APumpkinGameModeBase()
{
    CurrentGameState = EGameStates::Player1Turn;

	DefaultDamageValue = -1;
}

void APumpkinGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void APumpkinGameModeBase::SwitchTurn()
{
    // Switch Current Game State
    switch (CurrentGameState)
    {
    case EGameStates::Player1Turn:
        // Switch to other player turn state
        CurrentGameState = EGameStates::Player2Turn;
        break;
    case EGameStates::Player2Turn:
        // Switch to other turn state
        CurrentGameState = EGameStates::Player1Turn;

        break;
    default:

        break;
    }
}

void APumpkinGameModeBase::BulletFired(APawn* HoldingPawn, APawn* HitPawn, bool bLiveBullet)
{
	if(!HitPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitpawn not valid"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, "BulletFired");
	UE_LOG(LogTemp, Warning, TEXT("BulletFired"));
	
	// if bullet is live then deal damage

	if (HoldingPawn == HitPawn)
	{
		if (!bLiveBullet)
		{
			// @TODO (Denis): Might need to reset turn here
			return;
		}
	
	}

	// Apply Damage is live bullet
	if (bLiveBullet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dealing %d damage") , DefaultDamageValue);
		// apply damage to player
		UPumpkinHealthComponent* HitPawnHealthComponent = Cast<UPumpkinHealthComponent>(HitPawn->FindComponentByClass<UPumpkinHealthComponent>());
		HitPawnHealthComponent->AdjustHealth(DefaultDamageValue); // take 1 health off
	}

	SwitchTurn();
}

bool APumpkinGameModeBase::CanFire(APawn* HoldingPawn) const
{
	if (CurrentGameState == EGameStates::Player1Turn)
	{
		return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 1;
	}

	return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 2;
}

int32 APumpkinGameModeBase::RequestPlayerIndex()
{
	return ++LastPlayerIndex;
}
