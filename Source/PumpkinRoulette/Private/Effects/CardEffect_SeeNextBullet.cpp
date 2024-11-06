// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SeeNextBullet.h"

#include "PumpkinPlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PumpkinRoulette/Public/PumpkinGun.h"

void UCardEffect_SeeNextBullet::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	const AActor* ActorGun = UGameplayStatics::GetActorOfClass(OwningPawn->GetWorld(), APumpkinGun::StaticClass());
	if (const APumpkinGun* PlayerGun = Cast<APumpkinGun>(ActorGun))
	{
		const FString NextBullet = PlayerGun->GetNextBullet() ?  FString("Live") : FString("Blank");
		IPumpkinPlayerInterface::Execute_DisplayNotification(OwningPawn, NextBullet);
		IPumpkinPlayerInterface::Execute_DisplayNotification(TargetPawn, FString("Sees next bullet"));
	}
}
