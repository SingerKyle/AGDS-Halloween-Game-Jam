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
		APawn* HitPawn = Cast<APawn>(OutHit.GetActor());
		APawn* HoldingPawn = GetHoldingPawn();

		GameMode->BulletFired(HoldingPawn, HitPawn, Data.bLiveBullet);

		if (Bullets.Num() == 0)
		{
			ReloadGun();
		}
		
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
	const int32 NumDeadBullets = 6 - NumLiveBullets;
	
	for (int32 Idx = 0; Idx < NumLiveBullets; ++Idx)
	{
		FBulletData Data;
		Data.bLiveBullet = true;
		Bullets.Add(Data);
	}
	for (int32 Idx = 0; Idx < NumDeadBullets; ++Idx)
	{
		FBulletData Data;
		Data.bLiveBullet = false;
		Bullets.Add(Data);
	}

	FMath::SRandInit(FMath::Rand());
	// Shuffle Array
	Algo::Sort(Bullets, [](const FBulletData& A, const FBulletData& B)
	{
		return FMath::SRand() > 0.5f;
	});

	APumpkinGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APumpkinGameModeBase>();
	const FString Message = FString::Printf(TEXT("Live: %d, Blank: %d"), NumLiveBullets, NumDeadBullets); 
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
