// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BouncingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class EPHESUS_API ABouncingEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABouncingEnemy();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	class AMainCharacter* MainCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void StopMovement();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

