// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PumpkinHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUMPKINROULETTE_API UPumpkinHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Replicated) int PlayerHealth;
public:	
	// Sets default values for this component's properties
	UPumpkinHealthComponent();

	void AdjustHealth(int HealthChange);
	UFUNCTION(Server, Reliable) void ServerAdjustHealth(int HealthChange);
	FORCEINLINE int GetHealth() { return PlayerHealth; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
