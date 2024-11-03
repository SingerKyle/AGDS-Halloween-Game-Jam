// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grippables/GrippableActor.h"
#include "PumpkinGun.generated.h"

// @NOTE (Denis): This might not be required, but it could come in handy in the future
USTRUCT(BlueprintType)
struct FBulletData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "BulletData")
	bool bLiveBullet = false;
};

UCLASS()
class PUMPKINROULETTE_API APumpkinGun : public AGrippableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APumpkinGun(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void FireBullet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE bool GetNextBullet() { return Bullets[1].bLiveBullet; };
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReloadGun();

	UFUNCTION(Server, Reliable)
	void ServerReloadGun();

	UFUNCTION(Server, Reliable)
	void ServerFireBullet();

	UFUNCTION()
	void OnRep_BulletData();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastPostReload(int32 LiveBullets, int32 DeadBullets);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastBulletFired(bool bLiveBullet);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastBulletMisfired();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APawn* GetHoldingPawn();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunConfig")
	float GunRange = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunConfig")
	float TraceRadius = 64.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GunConfig")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToTrace;

	UPROPERTY(EditDefaultsOnly, Category = "GunConfig")
	TObjectPtr<USoundBase> MisfireSound;

	UPROPERTY(EditDefaultsOnly, Category = "GunConfig")
	TObjectPtr<USoundBase> GunFireSound;
	
private:

	UPROPERTY(ReplicatedUsing = OnRep_BulletData)
	TArray<FBulletData> Bullets;

};
