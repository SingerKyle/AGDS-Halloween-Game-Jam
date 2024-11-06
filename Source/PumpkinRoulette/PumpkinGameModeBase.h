// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PumpkinGameModeBase.generated.h"

class APumpkinCard;
class UPumpkinCardData;
class APumpkinCardHolder;
class APumpkinGun;

UENUM(BlueprintType)
enum class EGameStates : uint8
{
	None = 0,
	Player1Turn = 1		UMETA(DisplayName = "Player 1's Turn"),
	Player2Turn = 2		UMETA(DisplayName = "Player 2's Turn"),

	GameEnd
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

	bool IsPlayersTurn(APawn* HoldingPawn) const;

	void RegisterGun(APumpkinGun* TheGun);
	
	UFUNCTION(BlueprintCallable, Category = Game, meta = (DefaultToSelf = Pawn))
	int32 RequestPlayerIndex(APawn* Pawn);
	
	// Getter for the current game state
	EGameStates GetCurrentGameState() const { return CurrentGameState; }

	void SetDamageModifier(int NewDamageModifier);

	void SetSkipNextTurn(bool Val);
	FORCEINLINE bool GetSkipNextTurn() { return bSkipNextTurn; }

	void SetNextLiveBulletWin(bool Val);
	FORCEINLINE bool GetNextLiveBulletWin() { return bNextLiveBulletWin; }

	void SetWildCardDamageOrHeal(bool Val);
	FORCEINLINE bool GetWildCardDamageOrHeal() { return bWildCardDamageOrHeal; }

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	APawn* GetOtherPlayer(const APawn* Player) const;

	void SpawnCard();

	void RouteMessageToBothPlayers(const FString& Message);
	void OnPlayerDead(APawn* Pawn);

protected:

	UPROPERTY()
	APumpkinGun* Gun;

	UPROPERTY()
	int32 LastPlayerIndex = 0;

	UPROPERTY()
	APawn* Pawn1;
	
	UPROPERTY()
	APawn* Pawn2;

	UPROPERTY()
	TArray<APumpkinCardHolder*> Player1CardHolders;

	UPROPERTY()
	TArray<APumpkinCardHolder*> Player2CardHolders;

	UPROPERTY(EditDefaultsOnly)
	TArray<UPumpkinCardData*> CardDatas;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APumpkinCard> CardTemplate;
	
	UPROPERTY(BlueprintReadWrite, Category = "Damage") int DefaultDamageValue;
	UPROPERTY(BlueprintReadWrite, Category = "Damage")int DamageModifier;
	
	// variable for skip turn card.
	bool bSkipNextTurn;
	// variable for instakill
	bool bNextLiveBulletWin;
	// variable for wildcard damage / healing
	bool bWildCardDamageOrHeal;

	int32 LastPlayerStartUsed = 0;

	// Current game state
	UPROPERTY(BlueprintReadOnly, Category = "Game State") EGameStates CurrentGameState;

private:

	APumpkinCardHolder* TryFindFreeCardHolder(const TArray<APumpkinCardHolder*>& CardHolders) const;
	UPumpkinCardData* GetRandomCardData() const;
};
