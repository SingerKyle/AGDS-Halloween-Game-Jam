// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grippables/GrippableActor.h"
#include "PumpkinCard.generated.h"

class UPumpkinCardData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardPlayedSignature);

UCLASS()
class PUMPKINROULETTE_API APumpkinCard : public AGrippableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APumpkinCard(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCardData(UPumpkinCardData* NewCardData);

	UFUNCTION(BlueprintCallable)
	void PlayCard(APawn* PawnInstigator, APawn* Target);

	UFUNCTION(Server, Reliable)
	void ServerPlayCard(APawn* PawnInstigator, APawn* Target);

	void SetCardSlotLocation(const FTransform& NewCardSlotLocation);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation) override;

	virtual void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed) override;

public:

	UPROPERTY()
	FOnCardPlayedSignature OnCardPlayed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:

	UPROPERTY()
	UGripMotionControllerComponent* GrippingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(EditDefaultsOnly)
	FVector ActorBoxExtends;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;	

	UPROPERTY()
	FTransform CardSlotLocation;
	
private:

	UFUNCTION()
	void OnRep_CardData();
	
private:

	UPROPERTY(ReplicatedUsing = OnRep_CardData)
	UPumpkinCardData* CardData;

};
