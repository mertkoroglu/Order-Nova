// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncingEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABouncingEnemy::ABouncingEnemy() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	ProjectileMovement->Friction = 0.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.f;
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.f;

}

// Called when the game starts or when spawned
void ABouncingEnemy::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (MainCharacter) {
		MainCharacter->OnDying.AddDynamic(this, &ABouncingEnemy::StopMovement);
	}
}

void ABouncingEnemy::StopMovement()
{
	ProjectileMovement->StopMovementImmediately();
	UE_LOG(LogTemp, Warning, TEXT("Should Stp"));
}

// Called every frame
void ABouncingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

