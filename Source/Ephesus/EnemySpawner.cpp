// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemySpawnIndicator.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacter.h"
#include "BouncingEnemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner():
	EnemySpawnRate(3.f),
    bStopEverything(false)

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (NavSys)
        FindRandomLocation();

    if (MainCharacter) {
        MainCharacter->OnDying.AddDynamic(this, &AEnemySpawner::Stop);
    }
	
}

void AEnemySpawner::SpawnEnemy()
{
    if (!bStopEverything) {

        float randomYaw = FMath::RandRange(0, 360);

        GetWorld()->SpawnActor<ABouncingEnemy>(BouncingEnemy, RandomPoint.Location, FRotator(0.f, randomYaw, 0.f), FActorSpawnParameters());
    }
}

void AEnemySpawner::WaitForSpawn()
{
    if(!bStopEverything)
        GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemySpawner::SpawnEnemy, 2.f, false);
}

void AEnemySpawner::WaitForNewLocation()
{
    GetWorldTimerManager().SetTimer(NewLocationTimer, this, &AEnemySpawner::FindRandomLocation, EnemySpawnRate, false);
}

void AEnemySpawner::SpawnIndicator()
{
    GetWorld()->SpawnActor<AEnemySpawnIndicator>(EnemySpawnIndicator, RandomPoint.Location, FRotator(0.f, 0.f, 0.f), FActorSpawnParameters());
    if (!bStopEverything)
        WaitForSpawn();
}

void AEnemySpawner::FindRandomLocation()
{
    if (!bStopEverything) {
        // Get a random point from the navigation system
        if (NavSys->GetRandomPoint(RandomPoint))
        {
            // Log the random point for debugging purposes
            UE_LOG(LogTemp, Warning, TEXT("Random Nav Point: %s"), *RandomPoint.Location.ToString());
            // Example of spawning an enemy at the random point
            SpawnIndicator();
        }

        WaitForNewLocation();
    }
}

void AEnemySpawner::Stop()
{
    bStopEverything = true;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

