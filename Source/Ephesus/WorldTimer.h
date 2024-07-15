// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameTimerEnd);


UCLASS()
class EPHESUS_API AWorldTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldTimer();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();
	void TimerComplete();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	FString GetRemainingTime();

	UPROPERTY(BlueprintAssignable)
	FOnGameTimerEnd OnGameTimerEnd;
private:

	FTimerHandle TimerHandle;
	float StartTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer Settings", meta = (AllowPrivateAccess = "true"))
	float Duration;

};
