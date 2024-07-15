// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile() :
	bBounced(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement->Friction = 0.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.f;
	ProjectileMovement->InitialSpeed = 900.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (CollisionBox) {
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	}
	if (MainCharacter)
	{
		// Bind to the OnDying delegate
		MainCharacter->OnDying.AddDynamic(this, &AProjectile::StopMovement);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (bBounced)
		Destroy();
	else {
		if(BouncedMaterial)
			Mesh->SetMaterial(0, BouncedMaterial);
		bBounced = true;
	}
	
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy"))) {
		Enemy = Cast<AEnemy>(OtherActor);
		Enemy->ReceiveDamage();
		Destroy();
	}
	else if (OtherActor->ActorHasTag(TEXT("Character")) && bBounced) {
		MainCharacter->TakeDamage();
	}

}

void AProjectile::StopMovement()
{
	ProjectileMovement->StopMovementImmediately();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

