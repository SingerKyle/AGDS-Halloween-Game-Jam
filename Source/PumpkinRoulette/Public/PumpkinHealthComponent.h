// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PumpkinHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int32, NewHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUMPKINROULETTE_API UPumpkinHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPumpkinHealthComponent();

	void AdjustHealth(int HealthChange);
	UFUNCTION(Server, Reliable) void ServerAdjustHealth(int HealthChange);
	void Instakill();
	UFUNCTION(Server, Reliable) void ServerInstakill();
	FORCEINLINE int GetHealth() const { return PlayerHealth; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PlayerHealth();

private:

	UPROPERTY(ReplicatedUsing = OnRep_PlayerHealth) int PlayerHealth;
	
};
