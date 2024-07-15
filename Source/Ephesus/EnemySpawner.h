// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class EPHESUS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
private:
	FTimerHandle EnemySpawnTimer;
	FTimerHandle NewLocationTimer;
	class UNavigationSystemV1* NavSys;
	FNavLocation RandomPoint;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABouncingEnemy> BouncingEnemy;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEnemySpawnIndicator> EnemySpawnIndicator;
	class AMainCharacter* MainCharacter;
	bool bStopEverything;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer Settings", meta = (AllowPrivateAccess = "true"))
	float EnemySpawnRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnEnemy();
	void WaitForSpawn();
	void WaitForNewLocation();

	void SpawnIndicator();
	void FindRandomLocation();
	UFUNCTION()
	void Stop();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
