// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameModeBase.h"

#include "PumpkinCard.h"
#include "PumpkinCardHolder.h"
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

	Player1CardHolders = {nullptr, nullptr, nullptr, nullptr, nullptr};
	Player2CardHolders = {nullptr, nullptr, nullptr, nullptr, nullptr};
}

void APumpkinGameModeBase::BeginPlay()
{
    Super::BeginPlay();

	TArray<AActor*> CardHolders;
	UGameplayStatics::GetAllActorsOfClass(this, APumpkinCardHolder::StaticClass(), CardHolders);

	for (const auto& CardHolder : CardHolders)
	{
		APumpkinCardHolder* Holder = Cast<APumpkinCardHolder>(CardHolder);
		if (Holder->CardHolderType == ECardHolderType::Player1)
		{
			Player1CardHolders[Holder->CardSlotIndex] = Holder;
		}
		else
		{
			Player2CardHolders[Holder->CardSlotIndex] = Holder;
		}
	}
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
        	Gun->SetActorRotation(FRotator(180.f, 0.f, 180.f));
        }
        CurrentGameState = EGameStates::Player2Turn;
    	IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn1, FString("Enemy turn!"));
    	IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn2, FString("Your turn!"));
        break;
    case EGameStates::Player2Turn:
        // Switch to other turn state
        if (Gun)
        {
        	Gun->SetActorLocation(FVector(234.f, 7.f, 172.f));
        	Gun->SetActorRotation(FRotator(0.f, 0.f, 0.f));
        }
        CurrentGameState = EGameStates::Player1Turn;
    	IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn1, FString("Your turn!"));
    	IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn2, FString("Enemy turn!"));
        break;
    default:
    	// Really bad, but it should work for now
    	if (Gun)
    	{
    		Gun->SetActorLocation(FVector(234.f, 7.f, 172.f));
    		Gun->SetActorRotation(FRotator(0.f, 0.f, 0.f));
    	}
    	CurrentGameState = EGameStates::Player1Turn;
    	if (Pawn1)
    	{
    		IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn1, FString("Your turn!"));
    	}
    	if (Pawn2)
    	{
    		IPumpkinPlayerInterface::Execute_DisplayNotification(Pawn2, FString("Enemy turn!"));
    	}
        break;
    }

	SpawnCard();
}

void APumpkinGameModeBase::BulletFired(APawn* HoldingPawn, APawn* HitPawn, bool bLiveBullet)
{
	if(!HitPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitpawn not valid"));
		return;
	}
	
	// if bullet is live then deal damage
	if (HoldingPawn == HitPawn)
	{
		if (!bLiveBullet)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shooting myself!"));
			// @TODO (Denis): Might need to reset turn here
			return;
		}
	
	}

	// Apply Damage is live bullet
	if (bLiveBullet)
	{
		if (bNextLiveBulletWin) // if the wildcard for instakill has been activated.
		{
			UE_LOG(LogTemp, Warning, TEXT("Instakill"));

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

bool APumpkinGameModeBase::IsPlayersTurn(APawn* HoldingPawn) const
{
	if (CurrentGameState == EGameStates::Player1Turn)
	{
		return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 1;
	}

	return IPumpkinPlayerInterface::Execute_GetPlayerIndex(HoldingPawn) == 2;
}

void APumpkinGameModeBase::RegisterGun(APumpkinGun* TheGun)
{
	Gun = TheGun;
}

int32 APumpkinGameModeBase::RequestPlayerIndex(APawn* Pawn)
{
	LastPlayerIndex += 1;
	LastPlayerIndex = FMath::Clamp(LastPlayerIndex, 1, 2);
	if (LastPlayerIndex == 1)
	{
		Pawn1 = Pawn;
	}
	else
	{
		Pawn2 = Pawn;
		SwitchTurn();
	}
	
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

APawn* APumpkinGameModeBase::GetOtherPlayer(const APawn* Player) const
{
	return Player == Pawn1 ? Pawn2 : Pawn1;
}

void APumpkinGameModeBase::SpawnCard()
{
	if (CurrentGameState == EGameStates::Player1Turn)
	{
		if (auto CardHolder = TryFindFreeCardHolder(Player1CardHolders))
		{
			APumpkinCard* Card = GetWorld()->SpawnActorDeferred<APumpkinCard>(CardTemplate, CardHolder->GetActorTransform(), Pawn1);
			Card->SetCardData(GetRandomCardData());
			Card->SetCardSlotLocation(CardHolder->GetActorTransform());
			CardHolder->SetCard(Card);
			Card->FinishSpawning(CardHolder->GetActorTransform());
		}
	}
	else if (CurrentGameState == EGameStates::Player2Turn)
	{
		if (auto CardHolder = TryFindFreeCardHolder(Player2CardHolders))
		{
			APumpkinCard* Card = GetWorld()->SpawnActorDeferred<APumpkinCard>(CardTemplate, CardHolder->GetActorTransform(), Pawn1);
			Card->SetCardData(GetRandomCardData());
			Card->SetCardSlotLocation(CardHolder->GetActorTransform());
			CardHolder->SetCard(Card);
			Card->FinishSpawning(CardHolder->GetActorTransform());
		}
	}
}

APumpkinCardHolder* APumpkinGameModeBase::TryFindFreeCardHolder(const TArray<APumpkinCardHolder*>& CardHolders) const
{
	for (const auto& CardHolder : CardHolders)
	{
		if (CardHolder->HasCard())
		{
			continue;
		}

		return CardHolder;
	}
	
	return nullptr;
}

UPumpkinCardData* APumpkinGameModeBase::GetRandomCardData() const
{
	return CardDatas[FMath::RandRange(0, CardDatas.Num() - 1)];
}
