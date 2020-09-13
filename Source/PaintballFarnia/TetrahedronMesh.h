// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TetrahedronMesh.generated.h"

UCLASS()
class PAINTBALLFARNIA_API ATetrahedronMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrahedronMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void GenerateTetraMesh();

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent*mesh;
};
