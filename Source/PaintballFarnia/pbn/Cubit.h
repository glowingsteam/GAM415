// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlowInterface.h"
#include "pbn/ColorPicker.h"
#include "Cubit.generated.h"

UCLASS()
class PAINTBALLFARNIA_API ACubit : public AActor, public IGlowInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubit();

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

	// Glow Interaction
	virtual void ChangeColor_Implementation(EColorPicker newColor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
		UMaterialInterface * Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Cyan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Magenta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Orange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Purple;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Brown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * Black;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface * White;

};

