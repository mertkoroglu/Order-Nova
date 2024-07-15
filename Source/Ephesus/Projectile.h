// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class EPHESUS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void StopMovement();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* FireEmitter;*/
	bool bBounced;
	class AEnemy* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CodeMaterials", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstance* BouncedMaterial;

	class AMainCharacter* MainCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
