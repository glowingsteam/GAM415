// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ColorPicker.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EColorPicker : uint8
{
	Red			UMETA(DisplayName = "Red"),
	Blue		UMETA(DisplayName = "Blue"),
	Green		UMETA(DisplayName = "Green"),
	Yellow		UMETA(DisplayName = "Yellow"),
	Cyan		UMETA(DisplayName = "Cyan"),
	Magenta		UMETA(DisplayName = "Magenta"),
	Orange		UMETA(DisplayName = "Orange"),
	Purple		UMETA(DisplayName = "Purple"),
	Brown		UMETA(DisplayName = "Brown"),
	Black		UMETA(DisplayName = "Black"),
	White		UMETA(DisplayName = "White")
};

UCLASS()
class PAINTBALLFARNIA_API UColorPicker : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};
