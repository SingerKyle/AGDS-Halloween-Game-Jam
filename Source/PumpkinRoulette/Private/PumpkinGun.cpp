// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinGun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PumpkinRoulette/PumpkinGameModeBase.h"

static TAutoConsoleVariable<int32> CVarShowDebugGunTrace(
	TEXT("ShowDebug.GunTrace"),
	0,
	TEXT("Shows debug for the gun trace"),
	ECVF_Cheat
);

// Sets default values
APumpkinGun::APumpkinGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void APumpkinGun::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APumpkinGun, Bullets);
}

// Called when the game starts or when spawned
void APumpkinGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APumpkinGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APumpkinGun::ReloadGun()
{
	ServerReloadGun();
}

void APumpkinGun::FireBullet()
{
	ServerFireBullet();
}

void APumpkinGun::ServerFireBullet_Implementation()
{
	APumpkinGameModeBase* GameMode = Cast<APumpkinGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode->CanFire(GetHoldingPawn()) == false)
	{
		NetMulticastBulletMisfired();
		return;
	}
	
	const FVector Forward = GetActorForwardVector();
	const FVector Start = GetActorLocation();
	const FVector End = Start + Forward * GunRange;
	
	const bool bDrawDebug = CVarShowDebugGunTrace.GetValueOnAnyThread() > 0;
	EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FHitResult OutHit;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, TraceRadius, ObjectTypesToTrace,
		false, {this}, DrawDebugType, OutHit, true))
	{
		// @TODO (Denis): Determine who is shooting and who is being shot, then let the GameMode know, also do some feedback
		FBulletData Data = Bullets[0];
		Bullets.RemoveAt(0);

		if (Bullets.Num() == 0)
		{
			ReloadGun();
		}

		APawn* HitPawn = Cast<APawn>(OutHit.GetActor());
		APawn* HoldingPawn = GetHoldingPawn();

		GameMode->BulletFired(HoldingPawn, HitPawn, Data.bLiveBullet);
		NetMulticastBulletFired(Data.bLiveBullet);
	}
	else
	{
		NetMulticastBulletMisfired();
	}

	
}

void APumpkinGun::ServerReloadGun_Implementation()
{
	Bullets.Empty();

	int32 LiveBullets = 0;
	int32 DeadBullets = 0;
	for (int32 i = 0; i < 6; ++i)
	{
		// How do we determine if a bullet is live
		FBulletData Data;
		Data.bLiveBullet = FMath::RandBool();
		LiveBullets += Data.bLiveBullet ? 1 : 0;
		DeadBullets += Data.bLiveBullet ? 0 : 1;
		
		Bullets.Add(Data);
	}

	NetMulticastPostReload(LiveBullets, DeadBullets);
}

void APumpkinGun::OnRep_BulletData()
{
	
}

void APumpkinGun::NetMulticastBulletFired_Implementation(bool bLiveBullet)
{
	// @TODO (Denis): Play an effect when a bullet has been fired
}

void APumpkinGun::NetMulticastBulletMisfired_Implementation()
{
	// @TODO (Denis): Play a misfire sound effect
}

void APumpkinGun::NetMulticastPostReload_Implementation(int32 LiveBullets, int32 DeadBullets)
{
	// @TODO: Show on screen how many live and dead bullets were reloaded
}

APawn* APumpkinGun::GetHoldingPawn_Implementation()
{
	return nullptr;
}
