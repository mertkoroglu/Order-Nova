// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "Engine/PostProcessVolume.h"
#include "WorldTimer.h"

#include "EngineUtils.h" 
#include "Sound/SoundCue.h"



// Sets default values
AMainCharacter::AMainCharacter() :
	bCanRotate(true),
	bActorHitWall(false),
	MaxHealth(3),
	CurrentHealth(3),
	bDead(false),
	bYouWin(false),
	InterpSpeed(3.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CurrentVelocity = FVector::ZeroVector;

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMesh"));
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);

}

void AMainCharacter::OnGameTimerEnd()
{
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	AngleToMouseYaw = FRotator{ 0,0,0 };
	CurrentVelocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->bUseControllerDesiredRotation = true; // Character moves in the direction of input...
	GetCharacterMovement()->BrakingFriction = 0.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	bUseControllerRotationYaw = false;
	MouseMesh->SetWorldLocation(FVector(0,0, 0));

	for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
	{
		PostProcessVolume = *It;
		break;
	}


	for (TActorIterator<AWorldTimer> It(GetWorld()); It; ++It)
	{
		WorldTimer = *It;
		break; 
	}

	WorldTimer->OnGameTimerEnd.AddDynamic(this, &AMainCharacter::YouWon);

	CurrentBloomIntensity = PostProcessVolume->Settings.BloomIntensity;
	CurrentExposureBias = PostProcessVolume->Settings.AutoExposureBias;
	CurrentSceneFringeIntensity = PostProcessVolume->Settings.SceneFringeIntensity;
}

void AMainCharacter::FindMouseHit(float DeltaTime)
{
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FCollisionQueryParams QueryParams;
	GetWorld()->LineTraceSingleByChannel(MouseHit, WorldLocation, WorldDirection * 10000.f + WorldLocation, ECC_Visibility);

	if (MouseHit.bBlockingHit)
	{
		MouseHitLoc = MouseHit.Location;

		// If character hit the wall bCanRotate will be false for a short while, disabling the mouse rotation effect.
		if (bCanRotate) {
			// Calculate the target rotation to face the mouse hit location
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(MouseHitLoc.X, MouseHitLoc.Y, GetActorLocation().Z));
			// Interpolate smoothly from current rotation to target rotation
			FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f); // Adjust the interp speed (5.0f) as needed
			SetActorRotation(NewRotation);
			CurrentVelocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
		}

		MouseMesh->SetWorldLocation(FVector(MouseHitLoc.X, MouseHitLoc.Y, 35.f));
	}
}

void AMainCharacter::EnableMouseHit()
{
	bCanRotate = true;
}

void AMainCharacter::InterpToWorldCenter(float DeltaTime)
{
	HitTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(0, 0, GetActorLocation().Z));
	FRotator NewRotationHit = FMath::RInterpTo(GetActorRotation(), HitTargetRotation, DeltaTime, 5.0f); // Adjust the interp speed (5.0f) as needed

	SetActorRotation(NewRotationHit);
	CurrentVelocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
}

void AMainCharacter::DisableWallHitRotation()
{
	bActorHitWall = false;
}

void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy"))) {
		TakeDamage();
	}
}

void AMainCharacter::TakeDamage()
{
	if (!bYouWin) {
		CurrentHealth--;
		UE_LOG(LogTemp, Warning, TEXT("You took Damage: Health = %d"), CurrentHealth);

		UGameplayStatics::PlaySound2D(GetWorld(), DamageSound);

		if (CurrentHealth == 0) {
			Die();
		}
	}
}

int32 AMainCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

void AMainCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("You Died")); 
	FPostProcessSettings& Settings = PostProcessVolume->Settings;

	// Example: Set color saturation to zero
	Settings.ColorSaturation = FVector4(0, 0, 0, 0);

	// Apply the settings to the post-process volume
	PostProcessVolume->Settings = Settings;

	OnDying.Broadcast();
	bDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCharacterMovement()->DisableMovement();

	GetWorldTimerManager().SetTimer(DieTimer, this, &AMainCharacter::YouLost, 3.f, false);
}

void AMainCharacter::Attack()
{
	if(!bDead)
		GetWorld()->SpawnActor<AProjectile>(Projectile, GetActorLocation() + GetActorForwardVector() * 10.f, GetActorRotation(), FActorSpawnParameters());
}

void AMainCharacter::YouLost()
{
	OnDeath.Broadcast();
}

void AMainCharacter::YouWon()
{
	if(!bDead){
		if (PostProcessVolume)
		{
			TargetBloomIntensity = 100.0f;
			TargetExposureBias = -10.0f;
			TargetSceneFringeIntensity = 5.0f;

			bYouWin = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PostProcessVolume is not set!"));
		}
	}
}

void AMainCharacter::FinishGame()
{
	OnWin.Broadcast();
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	bActorHitWall = true;
	// Disable Mouse hits rotation effect to character.
	bCanRotate = false;
	GetWorldTimerManager().SetTimer(DisableMouseRotationTimer, this, &AMainCharacter::EnableMouseHit, .3f, false);
	GetWorldTimerManager().SetTimer(WallHitTimer, this, &AMainCharacter::DisableWallHitRotation, .3f, false);
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead) {
		FindMouseHit(DeltaTime);

		// Gives the character forward movement indefinetely.
		AddMovementInput(FVector(CurrentVelocity.GetSafeNormal().X, CurrentVelocity.GetSafeNormal().Y, 0.f), 1.0f);

		// If actor hits the wall, rotate it to the world center.
		if (bActorHitWall) {
			InterpToWorldCenter(DeltaTime);
		}

		MouseMesh->SetWorldRotation(FRotator(0, 0, 0));
		//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 30.f));
	}


	if (bYouWin && PostProcessVolume)
	{
		FPostProcessSettings& Settings = PostProcessVolume->Settings;

		CurrentBloomIntensity = FMath::FInterpTo(CurrentBloomIntensity, TargetBloomIntensity, DeltaTime, InterpSpeed);
		CurrentExposureBias = FMath::FInterpTo(CurrentExposureBias, TargetExposureBias, DeltaTime, InterpSpeed);
		CurrentSceneFringeIntensity = FMath::FInterpTo(CurrentSceneFringeIntensity, TargetSceneFringeIntensity, DeltaTime, InterpSpeed);

		Settings.BloomIntensity = CurrentBloomIntensity;
		Settings.AutoExposureBias = CurrentExposureBias;
		Settings.SceneFringeIntensity = CurrentSceneFringeIntensity;

		PostProcessVolume->Settings = Settings;

		if (FMath::IsNearlyEqual(CurrentBloomIntensity, TargetBloomIntensity, 0.001f) &&
			FMath::IsNearlyEqual(CurrentExposureBias, TargetExposureBias, 0.001f) &&
			FMath::IsNearlyEqual(CurrentSceneFringeIntensity, TargetSceneFringeIntensity, 0.001f))
		{
			FinishGame();
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);


	PlayerInputComponent->BindAction(TEXT("Space"), IE_Pressed, this, &AMainCharacter::Attack);
}

