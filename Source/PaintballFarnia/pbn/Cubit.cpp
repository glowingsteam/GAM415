// Fill out your copyright notice in the Description page of Project Settings.

#include "Cubit.h"


// Sets default values
ACubit::ACubit()
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
void ACubit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubit::TriggerGlow_Implementation()
{
}

void ACubit::ChangeColor_Implementation(EColorPicker newColor)
{
	// Create temp variable and set material based on enum
	UMaterialInterface* interfaceToChange;
	switch (newColor)
	{
	case EColorPicker::Black:
		interfaceToChange = Black;
		break;
	case EColorPicker::Red:
		interfaceToChange = Red;
		break;
	case EColorPicker::Blue:
		interfaceToChange = Blue;
		break;
	case EColorPicker::Green:
		interfaceToChange = Green;
		break;
	case EColorPicker::Yellow:
		interfaceToChange = Yellow;
		break;
	case EColorPicker::Cyan:
		interfaceToChange = Cyan;
		break;
	case EColorPicker::Magenta:
		interfaceToChange = Magenta;
		break;
	case EColorPicker::Orange:
		interfaceToChange = Orange;
		break;
	case EColorPicker::Purple:
		interfaceToChange = Purple;
		break;
	case EColorPicker::Brown:
		interfaceToChange = Brown;
		break;
	case EColorPicker::White:
		interfaceToChange = White;
		break;
	default:
		interfaceToChange = White;
		break;
	}

	// Change Material of mesh to new color
	MeshComponent->SetMaterial(0, interfaceToChange);
}