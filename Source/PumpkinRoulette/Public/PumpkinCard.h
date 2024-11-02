// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PumpkinCard.generated.h"

class UPumpkinCardData;

UCLASS()
class PUMPKINROULETTE_API APumpkinCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APumpkinCard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCardData(UPumpkinCardData* NewCardData);

	UFUNCTION(BlueprintCallable)
	void PlayCard(APawn* PawnInstigator, APawn* Target);

	UFUNCTION(Server, Reliable)
	void ServerPlayCard(APawn* PawnInstigator, APawn* Target);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* CardMesh;
	
private:

	UFUNCTION()
	void OnRep_CardData();
	
private:

	UPROPERTY(ReplicatedUsing = OnRep_CardData)
	UPumpkinCardData* CardData;

};
