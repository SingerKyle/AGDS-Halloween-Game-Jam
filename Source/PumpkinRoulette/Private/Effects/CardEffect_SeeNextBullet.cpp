// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SeeNextBullet.h"
#include "Kismet/GameplayStatics.h"
#include "PumpkinRoulette/Public/PumpkinGun.h"

void UCardEffect_SeeNextBullet::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "See Next Bullet");

	AActor* ActorGun = UGameplayStatics::GetActorOfClass(GetWorld(), APumpkinGun::StaticClass());

	APumpkinGun* PlayerGun = Cast<APumpkinGun>(ActorGun);

	if (PlayerGun)
	{
		UE_LOG(LogTemp, Display, TEXT("Next bullet is: %d"), PlayerGun->GetNextBullet());
	}
}
