// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinGameEventsSubsystem.h"

void UPumpkinGameEventsSubsystem::SendMessage(EPlayer Player, FString Message)
{
	OnMessageReceived.Broadcast(Player, Message);
}
