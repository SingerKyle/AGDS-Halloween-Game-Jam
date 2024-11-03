// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PumpkinGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	None = 0,
	Player1Turn = 1		UMETA(DisplayName = "Player 1's Turn"),
	Player2Turn = 2		UMETA(DisplayName = "Player 2's Turn")
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

	void BulletFired(APawn* HoldingPawn, APawn* HitPawn, bool bLiveBullet);

	bool CanFire(APawn* HoldingPawn) const;

	void RegisterGun(AActor* TheGun);
	
	UFUNCTION(BlueprintCallable, Category = Game)
	int32 RequestPlayerIndex();
	
	// Getter for the current game state
	EGameStates GetCurrentGameState() const { return CurrentGameState; }

	void SetDamageModifier(int NewDamageModifier);

	void SetSkipNextTurn(bool Val);
	FORCEINLINE bool GetSkipNextTurn() { return bSkipNextTurn; }

	void SetNextLiveBulletWin(bool Val);
	FORCEINLINE bool GetNextLiveBulletWin() { return bNextLiveBulletWin; }

	void SetWildCardDamageOrHeal(bool Val);
	FORCEINLINE bool GetWildCardDamageOrHeal() { return bWildCardDamageOrHeal; }

protected:

	UPROPERTY()
	AActor* Gun;

	UPROPERTY()
	int32 LastPlayerIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Damage") int DefaultDamageValue;
	UPROPERTY(BlueprintReadWrite, Category = "Damage")int DamageModifier;

	// variable for skip turn card.
	bool bSkipNextTurn;
	// variable for instakill
	bool bNextLiveBulletWin;
	// variable for wildcard damage / healing
	bool bWildCardDamageOrHeal;

	// Current game state
	UPROPERTY(BlueprintReadOnly, Category = "Game State") EGameStates CurrentGameState;
};
