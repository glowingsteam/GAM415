// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlowInterface.h"
#include "GlowMesh.generated.h"

UCLASS()
class PAINTBALLFARNIA_API AGlowMesh : public AActor, public IGlowInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlowMesh();

	/** Root Guhponent */
	UPROPERTY(VisibleAnywhere)
		USceneComponent* rootSceneComp;

	/** Static Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Overlap")
		class UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Glow Interaction
	virtual void TriggerGlow_Implementation() override;

public:
	/** Glow Material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow Settings")
	UMaterialInterface* MaterialToInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow Settings")
	FLinearColor OriginalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow Settings")
	FLinearColor GlowColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow Settings")
	float GlowTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow Settings")
	float GlowIntensity;


private:
	// Private Glow Settings
	UMaterialInstanceDynamic * InstancedMaterial;
	float currentGlowTime;
	float currentGlowIntensity;
	
	
};
