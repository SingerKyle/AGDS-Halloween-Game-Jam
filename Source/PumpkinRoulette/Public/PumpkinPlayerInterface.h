// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PumpkinPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPumpkinPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUMPKINROULETTE_API IPumpkinPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisplayNotification(const FString& Notification);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExplodeHead();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DoJumpscare();
};
