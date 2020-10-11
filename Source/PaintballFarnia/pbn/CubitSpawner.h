// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubitSpawner.generated.h"

UCLASS()
class PAINTBALLFARNIA_API ACubitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubitSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnActorAtLocation(FVector spawnLocation, FRotator spawnRotation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	int xyAmt = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	float spawnOffset = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	AActor* actorToSpawn;
};
