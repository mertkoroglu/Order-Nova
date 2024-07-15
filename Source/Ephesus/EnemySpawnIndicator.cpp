// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawnIndicator.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawnIndicator::AEnemySpawnIndicator() :
    bStartedIndication(false)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpawnPointMesh");
    RootComponent = SpawnPointMesh;
}

// Called when the game starts or when spawned
void AEnemySpawnIndicator::BeginPlay()
{
    Super::BeginPlay();
    SetDynamicMaterial();
    StartHitFlash();
}

void AEnemySpawnIndicator::SetFlash()
{
    if (Curve && DynMaterial)
    {
        const float val = GetWorldTimerManager().GetTimerElapsed(CurveTimer);
        float CurveValue = Curve->GetFloatValue(val);
        DynMaterial->SetScalarParameterValue(TEXT("Opacity"), CurveValue);
    }
}

void AEnemySpawnIndicator::StartHitFlash()
{
    if (Curve)
    {
        bStartedIndication = true;
        GetWorldTimerManager().SetTimer(CurveTimer, this, &AEnemySpawnIndicator::EndHitFlash, 3.f, false);
    }
}

void AEnemySpawnIndicator::EndHitFlash()
{
    bStartedIndication = false;
    Destroy();
}

void AEnemySpawnIndicator::SetDynamicMaterial()
{
    Material = SpawnPointMesh->GetMaterial(0);
    DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
    SpawnPointMesh->SetMaterial(0, DynMaterial);
}

// Called every frame
void AEnemySpawnIndicator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bStartedIndication)
    {
        SetFlash();
    }
}
