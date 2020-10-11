// Fill out your copyright notice in the Description page of Project Settings.

#include "CubitSpawner.h"


// Sets default values
ACubitSpawner::ACubitSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ACubitSpawner::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < xyAmt; i++)
	{
		for (int j = 0; j < xyAmt; j++)
		{
			FVector currentLocation = GetActorLocation();
			FVector spawnLocation = FVector(currentLocation.X + (i *spawnOffset), currentLocation.Y + (j *spawnOffset), currentLocation.Z);
			SpawnActorAtLocation(spawnLocation, GetActorRotation());
		}
	}
}

// Called every frame
void ACubitSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

