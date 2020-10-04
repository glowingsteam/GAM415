// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowMesh.h"


// Sets default values
AGlowMesh::AGlowMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create scene component root
	rootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = rootSceneComp;

	// Create a Static Mesh	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	MeshComponent->SetSimulatePhysics(true);
	
}

// Called when the game starts or when spawned
void AGlowMesh::BeginPlay()
{
	Super::BeginPlay();

	// Create material and apply it
	InstancedMaterial = UMaterialInstanceDynamic::Create(MaterialToInstance, this);
	MeshComponent->SetMaterial(0, InstancedMaterial);

	// Set values to original
	InstancedMaterial->SetVectorParameterValue("Color", FVector(OriginalColor.R, OriginalColor.G, OriginalColor.B));
	InstancedMaterial->SetScalarParameterValue("Emissive", 1.0f);
}

// Called every frame
void AGlowMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentGlowTime > 0.0f)
	{
		// Subtract current time by deltatime
		currentGlowTime -= DeltaTime;
		
		// If time ran out reset color
		if (currentGlowTime <= 0.0f)
		{
			InstancedMaterial->SetVectorParameterValue("Color", FVector(OriginalColor.R, OriginalColor.G, OriginalColor.B));
			InstancedMaterial->SetScalarParameterValue("Emissive", 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("Reset Color"));
		}

		// Otherwise recalc intensity and apply
		else
		{
			currentGlowIntensity = (currentGlowTime / GlowTime) * GlowIntensity;
			InstancedMaterial->SetScalarParameterValue("Emissive", currentGlowIntensity);

			// Debug Glow time
			//UE_LOG(LogTemp, Warning, TEXT("Time Left: %f"), currentGlowTime);
		}
	}
}

void AGlowMesh::TriggerGlow_Implementation()
{
	// Set initial values and reset timer
	InstancedMaterial->SetVectorParameterValue("Color", FVector(GlowColor.R, GlowColor.G, GlowColor.B));
	InstancedMaterial->SetScalarParameterValue("Emissive", GlowIntensity);
	currentGlowIntensity = GlowIntensity;
	currentGlowTime = GlowTime;
}
