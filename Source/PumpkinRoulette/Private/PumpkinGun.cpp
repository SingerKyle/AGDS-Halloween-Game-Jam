// Fill out your copyright notice in the Description page of Project Settings.


#include "PumpkinGun.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
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
APumpkinGun::APumpkinGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);
	
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GetRootComponent());
	
	bReplicates = true;
}

void APumpkinGun::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APumpkinGun, Bullets);
}

void APumpkinGun::ForceDropGun_Implementation()
{
}

// Called when the game starts or when spawned
void APumpkinGun::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetAuthGameMode<APumpkinGameModeBase>()->RegisterGun(this);
		ReloadGun();
	}
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
	if (GameMode->IsPlayersTurn(GetHoldingPawn()) == false)
	{
		NetMulticastBulletMisfired();
		return;
	}
	
	const FVector Forward = GetActorForwardVector();
	const FVector Start = GetActorLocation() + (Forward * 10.f);
	const FVector End = Start + (Forward * GunRange);
	
	const bool bDrawDebug = CVarShowDebugGunTrace.GetValueOnAnyThread() > 0;
	EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FHitResult OutHit;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, TraceRadius, ObjectTypesToTrace,
		false, {this}, DrawDebugType, OutHit, true))
	{
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
	const int32 NumLiveBullets = FMath::RandRange(1, 3);
	
	for (int32 i = 0; i < 6; ++i)
	{
		// How do we determine if a bullet is live
		FBulletData Data;
		const bool bIsLive = FMath::RandBool();
		if (bIsLive && LiveBullets < NumLiveBullets)
		{
			Data.bLiveBullet = bIsLive;
			++LiveBullets;
		}
		else
		{
			Data.bLiveBullet = false;
			++DeadBullets;
		}
		
		Bullets.Add(Data);
	}

	APumpkinGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APumpkinGameModeBase>();
	const FString Message = FString::Printf(TEXT("Live: %d, Blank: %d"), LiveBullets, DeadBullets); 
	GameMode->RouteMessageToBothPlayers(Message);
}

void APumpkinGun::OnRep_BulletData()
{
	
}

void APumpkinGun::NetMulticastBulletFired_Implementation(bool bLiveBullet)
{
	if (bLiveBullet)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunFireSound, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FireParticles, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
	}
	if (!bLiveBullet)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunFireSoundBlank, GetActorLocation());
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SmokeParticles, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
}

void APumpkinGun::NetMulticastBulletMisfired_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, MisfireSound, GetActorLocation());
}

APawn* APumpkinGun::GetHoldingPawn_Implementation()
{
	return nullptr;
}
