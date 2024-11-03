// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameModeBase.h"

#include "PumpkinGun.h"
#include "PumpkinPlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "PumpkinHealthComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

APumpkinGameModeBase::APumpkinGameModeBase()
{
    CurrentGameState = EGameStates::None;

	DefaultDamageValue = -1;
	DamageModifier = 0;

	// variable for skip turn card.
	bSkipNextTurn = false;
	// variable for instakill
	bNextLiveBulletWin = false;
	bWildCardDamageOrHeal = false;
}

void APumpkinGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void APumpkinGameModeBase::SwitchTurn()
{
	Gun->ForceDropGun();
    // Switch Current Game State
    switch (CurrentGameState)
    {
    case EGameStates::Player1Turn:
        // Switch to other player turn state
        if (Gun)
        {
        	Gun->SetActorLocation(FVector(390.f, 7.f, 172.f));
        	Gun->SetActorRotation(FRotator(0, 0, 180));
        }
        CurrentGameState = EGameStates::Player2Turn;
        break;
    case EGameStates::Player2Turn:
        // Switch to other turn state
        if (Gun)
        {
        	Gun->SetActorLocation(FVector(234.f, 7.f, 172.f));
        	Gun->SetActorRotation(FRotator(0, 0, 180));
        }
        CurrentGameState = EGameStates::Player1Turn;

        break;
    default:
    	// Really bad, but it should work for now
    	if (Gun)
    	{
    		Gun->SetActorLocation(FVector(234.f, 7.f, 172.f));
    		Gun->SetActorRotation(FRotator(0, 0, 180));
    	}
    	CurrentGameState = EGameStates::Player1Turn;

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
		if (bNextLiveBulletWin) // if the wildcard for instakill has been activated.
		{
			UE_LOG(LogTemp, Warning, TEXT("Instakill"), DefaultDamageValue);

			// apply damage to player
			UPumpkinHealthComponent* HitPawnHealthComponent = Cast<UPumpkinHealthComponent>(HitPawn->FindComponentByClass<UPumpkinHealthComponent>());
			if (HitPawnHealthComponent)
			{
				HitPawnHealthComponent->Instakill(); // instakill
				// set back to 0 after card effect is used
				DamageModifier = 0;
			}

			bNextLiveBulletWin = false;
			return;
		}
		// add on any card damage here:
		int32 TotalDamage = DefaultDamageValue + DamageModifier;

		UE_LOG(LogTemp, Warning, TEXT("Dealing %d damage") , DefaultDamageValue);
		// apply damage to player
		UPumpkinHealthComponent* HitPawnHealthComponent = Cast<UPumpkinHealthComponent>(HitPawn->FindComponentByClass<UPumpkinHealthComponent>());
		if (HitPawnHealthComponent)
		{
			HitPawnHealthComponent->AdjustHealth(TotalDamage); // take health off
			// set back to 0 after card effect is used
			DamageModifier = 0;
		}
		
		

	}
	else if (!bLiveBullet && bWildCardDamageOrHeal) // if the bullet is fake AND wildcard to heal is active.
	{
		// Add healing to other player
	}

	// if card has not been played to skip turn - then move to next turn 
	if (!GetSkipNextTurn()) // NOTE: May need to create a reset turn function here depending.
	{
		SwitchTurn();
	}
	else
	{
		SetSkipNextTurn(false); // set variable back to false to move game on as normal.
	}
}

bool APumpkinGameModeBase::CanFire(APawn* HoldingPawn) const
{
	UE_LOG(LogTemp, Warning, TEXT("Turn is %s, and PlayerIndex is %d"), *UEnum::GetValueAsString(CurrentGameState), IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn));
	if (CurrentGameState == EGameStates::Player1Turn)
	{
		return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 1;
	}

	return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 2;
}

void APumpkinGameModeBase::RegisterGun(APumpkinGun* TheGun)
{
	Gun = TheGun;
	SwitchTurn();
}

int32 APumpkinGameModeBase::RequestPlayerIndex()
{
	LastPlayerIndex += 1;
	return LastPlayerIndex;
}

void APumpkinGameModeBase::SetDamageModifier(int NewDamageModifier)
{
	DamageModifier = NewDamageModifier;
}

void APumpkinGameModeBase::SetSkipNextTurn(bool Val)
{
	bSkipNextTurn = Val;
}

void APumpkinGameModeBase::SetNextLiveBulletWin(bool Val)
{
	bNextLiveBulletWin = Val;
}

void APumpkinGameModeBase::SetWildCardDamageOrHeal(bool Val)
{
	bWildCardDamageOrHeal = Val;
}

AActor* APumpkinGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);

	if (Actors.Num() > LastPlayerStartUsed)
	{
		return Actors[LastPlayerStartUsed++];
	}

	return nullptr;
}
