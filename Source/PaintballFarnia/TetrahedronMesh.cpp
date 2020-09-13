// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrahedronMesh.h"


// Sets default values
ATetrahedronMesh::ATetrahedronMesh()
{
	mesh = CreateDefaultSubobject  <UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;

}

// Called when the game starts or when spawned
void ATetrahedronMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATetrahedronMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATetrahedronMesh::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateTetraMesh();
}

void ATetrahedronMesh::PostLoad()
{
	Super::PostLoad();
	GenerateTetraMesh();
}

void ATetrahedronMesh::GenerateTetraMesh() 
{
	// Create base arrays
	TArray <FVector> Vertices;
	TArray <int32> Triangles;
	TArray <FVector> Normals;
	TArray < FLinearColor > Colors;

	// Add 4 verts
	Vertices.Add(FVector(-100.f, 100.f, -100.f));
	Vertices.Add(FVector(100.f, 100.f, -100.f));
	Vertices.Add(FVector(0.f, 0.f, 100.f));
	Vertices.Add(FVector(0.0f, -100.0f, -100.0f));

	// Side 1
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	// Side 2
	Triangles.Add(3);
	Triangles.Add(2);
	Triangles.Add(1);

	// Side 3
	Triangles.Add(1);
	Triangles.Add(0);
	Triangles.Add(3);

	// Bottom
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(3);

	
	//Triangles.Add(2);
	//Triangles.Add(3);
	//Triangles.Add(2);
	//Triangles.Add(1);

	// Add normals and colors
	for (int32 i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector(0.f, 0.f, 1.f));
		Colors.Add(FLinearColor::Red);
	} // Optional arrays.

	// Set UVs and color mesh
	TArray <FVector2D> UV0;
	TArray <FProcMeshTangent> Tangents;
	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, Colors, Tangents, true);
}
