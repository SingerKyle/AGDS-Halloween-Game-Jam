// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PumpkinGameEventsSubsystem.generated.h"

UENUM(BlueprintType)
enum class EPlayer : uint8
{
	Player1,
	Player2,
	Both
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerMessageEvent, EPlayer, Player, const FString&, Message);

/**
 * 
 */
UCLASS()
class PUMPKINROULETTE_API UPumpkinGameEventsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "PumpkinGameEventsSubsystem")
	void SendMessage(EPlayer Player, FString Message);

	UPROPERTY(BlueprintAssignable, Category = "PumpkinGameEventsSubsystem")
	FPlayerMessageEvent OnMessageReceived;
};
