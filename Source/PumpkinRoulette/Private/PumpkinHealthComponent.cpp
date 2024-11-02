// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinHealthComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UPumpkinHealthComponent::UPumpkinHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	PlayerHealth = 10;
}

void UPumpkinHealthComponent::AdjustHealth(int HealthChange)
{
	ServerAdjustHealth_Implementation(HealthChange);
}

void UPumpkinHealthComponent::ServerAdjustHealth_Implementation(int HealthChange)
{
	if (HealthChange < 0)
	{
		PlayerHealth += HealthChange;
	}
	else
	{
		PlayerHealth -= HealthChange;
	}
}


void UPumpkinHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPumpkinHealthComponent, PlayerHealth);
}

// Called when the game starts
void UPumpkinHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UPumpkinHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

