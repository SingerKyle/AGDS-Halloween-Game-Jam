// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PumpkinGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	Player1Turn		UMETA(DisplayName = "Player 1's Turn"),
	Player2Turn		UMETA(DisplayName = "Player 2's Turn")
};

/**
 *
 */
UCLASS()
class PUMPKINROULETTE_API APumpkinGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APumpkinGameModeBase();

	virtual void BeginPlay() override;

	/// <summary>
	/// Switch Player Turns
	/// </summary>
	void SwitchTurn();

	// Getter for the current game state
	EGameStates GetCurrentGameState() const { return CurrentGameState; }

protected:
	// variables for both players
	UPROPERTY(BlueprintReadOnly, Category = "Players") APawn* Player1;
	UPROPERTY(BlueprintReadOnly, Category = "Players") APawn* Player2;

	// Current game state
	UPROPERTY(BlueprintReadOnly, Category = "Game State") EGameStates CurrentGameState;
};
