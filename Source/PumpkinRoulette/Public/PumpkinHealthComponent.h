// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PumpkinHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUMPKINROULETTE_API UPumpkinHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPumpkinHealthComponent();

	void AdjustHealth(int HealthChange);
	UFUNCTION(Server, Reliable) void ServerAdjustHealth(int HealthChange);
	FORCEINLINE int GetHealth() const { return PlayerHealth; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PlayerHealth(int OldValue);

private:

	UPROPERTY(ReplicatedUsing = OnRep_PlayerHealth) int PlayerHealth;
	
};
