// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinHealthComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UPumpkinHealthComponent::UPumpkinHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
	PlayerHealth = 10;
}

void UPumpkinHealthComponent::AdjustHealth(int HealthChange)
{
	ServerAdjustHealth(HealthChange);
}

void UPumpkinHealthComponent::Instakill()
{
	ServerInstakill();
}

void UPumpkinHealthComponent::ServerInstakill_Implementation()
{
	// kill player instantly
	PlayerHealth = 0;

	OnRep_PlayerHealth(PlayerHealth);
	if(PlayerHealth == 0)
	{
		// Player Lost
	}
}

void UPumpkinHealthComponent::ServerAdjustHealth_Implementation(int HealthChange)
{
	const int OldHealth = PlayerHealth;
	PlayerHealth += HealthChange;
	
	OnRep_PlayerHealth(OldHealth);
	if (PlayerHealth == 0)
	{
		// Player lost
	}
}

void UPumpkinHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPumpkinHealthComponent, PlayerHealth);
}

// Called when the game starts
void UPumpkinHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPumpkinHealthComponent::OnRep_PlayerHealth(int OldValue)
{
	// Do UI stuff
}
