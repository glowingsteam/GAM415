// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcGenMesh.h"


// Sets default values
AProcGenMesh::AProcGenMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;

}

// Called when the game starts or when spawned
void AProcGenMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProcGenMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProcGenMesh::PostActorCreated() 
{
	Super::PostActorCreated();
	CreateSquare();
}

void AProcGenMesh::PostLoad()
{
	Super::PostLoad();
	CreateSquare();
}
void AProcGenMesh::CreateSquare()
{
	TArray <FVector> Vertices;
	TArray <int32> Triangles;
	TArray <FVector> Normals;
	TArray < FLinearColor > Colors;
	Vertices.Add(FVector(0.f, 0.f, 0.f));
	Vertices.Add(FVector(0.f, 100.f, 0.f));
	Vertices.Add(FVector(0.f, 0.f, 100.f));
	Vertices.Add(FVector(0.f, 100.f, 100.f));
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);
	Triangles.Add(2);
	Triangles.Add(1);
	for (int32 i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector(0.f, 0.f, 1.f));
		Colors.Add(FLinearColor::Red);
	} // Optional arrays.
	TArray <FVector2D> UV0;
	TArray <FProcMeshTangent> Tangents;
	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, Colors, Tangents, true);
}

