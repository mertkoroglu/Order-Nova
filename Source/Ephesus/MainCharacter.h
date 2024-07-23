// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDying);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWin);


UCLASS()
class EPHESUS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

private:
	int32 CurrentHealth;
	int32 MaxHealth;
	FVector CurrentVelocity;
	FVector CurrentDirection;
	float MoveSpeed;
	FRotator AngleToMouseYaw;
	APlayerController* PlayerController;
	FVector WorldLocation;
	FVector WorldDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator AngleToMouse;
	// Yaw Angle Between Mouse and the Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	
	FHitResult MouseHit;
	FVector MouseHitLoc;
	FTimerHandle DisableMouseRotationTimer;
	FTimerHandle WallHitTimer;
	FTimerHandle DieTimer;
	FTimerHandle ResetDamageTimer;

	bool bCanRotate;

	FRotator HitTargetRotation;
	bool bIsRotating = false;
	float RotationSpeed = 2.0f;  // Adjust as needed for smoother rotation
	bool bActorHitWall;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MouseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AProjectile> Projectile;

	class APostProcessVolume* PostProcessVolume;
	bool bDead;
	bool bYouWin;

	UFUNCTION()
	void OnGameTimerEnd();

	class AWorldTimer* WorldTimer;
	float TargetBloomIntensity;
	float TargetExposureBias;
	float TargetSceneFringeIntensity;

	float CurrentBloomIntensity;
	float CurrentExposureBias;
	float CurrentSceneFringeIntensity;

	float InterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* DamageSound;
	bool bCanGetDamaged;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// Each frame check mouse hit.
	void FindMouseHit(float DeltaTime);
	// Reenables the mouse hit rotation to character after wall hit effect is done.
	void EnableMouseHit();
	// If character hits a wall, give rotation to world center and give it velocity.
	void InterpToWorldCenter(float DeltaTime);
	// After the hit rotation is done, disable the effect.
	void DisableWallHitRotation();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void Die();
	void Attack();
	void YouLost();
	UFUNCTION()
	void YouWon();
	void FinishGame();
	void UpdateCanGetDamaged();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TakeDamage();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHealth();

	UPROPERTY(BlueprintAssignable)
	FOnDying OnDying;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;


	UPROPERTY(BlueprintAssignable)
	FOnWin OnWin;
};
