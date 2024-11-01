// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpkinGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>
#include "GameFramework/Pawn.h"

APumpkinGameModeBase::APumpkinGameModeBase()
{
    CurrentGameState = EGameStates::Player1Turn;
}

void APumpkinGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Cast to player characters
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), PlayerActors);

    if (PlayerActors.Num() >= 2)
    {
        Player1 = Cast<APawn>(PlayerActors[0]);
        Player2 = Cast<APawn>(PlayerActors[1]);
    }

    SwitchTurn();
}
/// <summary>
/// Switch Player Turns
/// </summary>
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

    OnRep_CurrentGameState();
}

void APumpkinGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APumpkinGameModeBase, Player1);
    DOREPLIFETIME(APumpkinGameModeBase, Player2);
    DOREPLIFETIME(APumpkinGameModeBase, CurrentGameState);
}

void APumpkinGameModeBase::OnRep_CurrentGameState()
{
    // Log the current turn
    UE_LOG(LogTemp, Log, TEXT("It is: %s"), *UEnum::GetDisplayValueAsText(CurrentGameState).ToString());


}
