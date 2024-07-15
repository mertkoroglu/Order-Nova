// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnIndicator.generated.h"

UCLASS()
class EPHESUS_API AEnemySpawnIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnIndicator();
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SpawnPointMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Curves, meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* Curve;
	FTimerHandle CurveTimer;


	class UMaterialInterface* Material;
	class UMaterialInstanceDynamic* DynMaterial;
	bool bStartedIndication;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetFlash();
	void StartHitFlash();
	void SetDynamicMaterial();
	void EndHitFlash();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
