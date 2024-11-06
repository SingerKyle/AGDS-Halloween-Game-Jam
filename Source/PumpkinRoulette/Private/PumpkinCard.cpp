// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/PumpkinCard.h"

#include "GripMotionControllerComponent.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"
#include "PumpkinRoulette/Public/PumpkinCardData.h"
#include "PumpkinRoulette/Public/PumpkinCardEffect.h"

// Sets default values
APumpkinCard::APumpkinCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>("CardMesh");
}

void APumpkinCard::SetCardSlotLocation(const FTransform& NewCardSlotLocation)
{
	CardSlotLocation = NewCardSlotLocation;
	SetActorTransform(NewCardSlotLocation);
}

void APumpkinCard::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APumpkinCard, CardData);
}

void APumpkinCard::OnGrip_Implementation(UGripMotionControllerComponent* GrippingController,
	const FBPActorGripInformation& GripInformation)
{
	Super::OnGrip_Implementation(GrippingController, GripInformation);

	if (!HasAuthority())
	{
		return;
	}
	
	GrippingComponent = GrippingController;
}

void APumpkinCard::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController,
	const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	Super::OnGripRelease_Implementation(ReleasingController, GripInformation, bWasSocketed);

	if (!HasAuthority())
	{
		return;
	}

	APumpkinGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APumpkinGameModeBase>();
	APawn* InstigatorPawn = Cast<APawn>(GrippingComponent->GetOwner());

	if (!GameMode->IsPlayersTurn(InstigatorPawn))
	{
		SetActorTransform(CardSlotLocation);
		ClientResetCard(CardSlotLocation);
		return;
	}
	
	TArray<AActor*> OverlappingActors;
	if (UKismetSystemLibrary::BoxOverlapActors(this, GetActorLocation(), ActorBoxExtends,
		ObjectTypes, ATriggerBox::StaticClass(), { this }, OverlappingActors))
	{
		if (OverlappingActors.Num() > 0)
		{

			APawn* OtherPawn = Cast<APawn>(GameMode->GetOtherPlayer(InstigatorPawn));
			PlayCard(InstigatorPawn, OtherPawn);
			return;
		}
	}

	// Reset card position, because it was incorrect placement
	SetActorTransform(CardSlotLocation);
	ClientResetCard(CardSlotLocation);
}

void APumpkinCard::ClientResetCard_Implementation(FTransform Transform)
{
	SetActorTransform(Transform);
}

// Called when the game starts or when spawned
void APumpkinCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APumpkinCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APumpkinCard::SetCardData(UPumpkinCardData* NewCardData)
{
	CardData = NewCardData;
	OnRep_CardData();
}

void APumpkinCard::PlayCard(APawn* PawnInstigator, APawn* Target)
{
	ServerPlayCard(PawnInstigator, Target);
}

void APumpkinCard::ServerPlayCard_Implementation(APawn* PawnInstigator, APawn* Target)
{
	OnCardPlayed.Broadcast();
	for (const auto Effect : CardData->Effects)
	{
		if (Effect)
		{
			Effect->Execute(PawnInstigator, Target);
		}
	}

	Destroy();
}

void APumpkinCard::OnRep_CardData()
{
	CardMesh->SetMaterial(0, CardData->Material);
	CardMesh->SetMaterial(1, CardData->Material);
}
