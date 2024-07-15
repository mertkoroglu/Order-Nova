// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTimer.h"

// Sets default values
AWorldTimer::AWorldTimer():
    Duration(120.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldTimer::BeginPlay()
{
	Super::BeginPlay();
    StartTimer();
}

// Called every frame
void AWorldTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldTimer::StartTimer()
{
    StartTime = GetWorld()->GetTimeSeconds();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWorldTimer::TimerComplete, Duration, false);
}

void AWorldTimer::TimerComplete()
{
    OnGameTimerEnd.Broadcast();
}

FString AWorldTimer::GetRemainingTime()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float ElapsedTime = CurrentTime - StartTime;
    float RemainingTime = Duration - ElapsedTime;

    if (RemainingTime < 0.0f)
    {
        RemainingTime = 0.0f;
    }

    int32 Minutes = FMath::FloorToInt(RemainingTime / 60.0f);
    int32 Seconds = FMath::FloorToInt(RemainingTime) % 60;

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
