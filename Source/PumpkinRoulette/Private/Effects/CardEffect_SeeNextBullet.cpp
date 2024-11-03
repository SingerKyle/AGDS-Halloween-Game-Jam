// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CardEffect_SeeNextBullet.h"
#include "Kismet/GameplayStatics.h"
#include "PumpkinRoulette/Public/PumpkinGun.h"

void UCardEffect_SeeNextBullet::Execute(APawn* OwningPawn, APawn* TargetPawn)
{
	Super::Execute(OwningPawn, TargetPawn);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Black, "See Next Bullet");

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APumpkinGun::StaticClass(), Found);

	for (int i = 0; i < Found.Num(); i++)
	{
		APumpkinGun* gun = Cast<APumpkinGun>(Found[i]);
		if (gun)
		{
			UE_LOG(LogTemp, Display, TEXT("Next bullet is: %d"), gun->GetNextBullet());
		}
	}
}
