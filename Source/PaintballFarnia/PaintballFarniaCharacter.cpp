// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "PaintballFarniaCharacter.h"
#include "PaintballFarniaProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GlowInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APaintballFarniaCharacter

APaintballFarniaCharacter::APaintballFarniaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Gun->SetupAttachment(RootComponent);

	// Init Shader vals
	EndColorBuildup = 0;
	EndColorBuildupDirection = 1;
	PixelShaderTopLeftColor = FColor::Green;
	ComputeShaderSimulationSpeed = 1.0;
	ComputeShaderBlend = 0.5f;
	ComputeShaderBlendScalar = 0;
	TotalElapsedTime = 0;

	// Init player muzzle & proj spawn loc
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void APaintballFarniaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Spawn fog and set ref
	m_fog = GetWorld()->SpawnActor<AFog>(AFog::StaticClass());
	m_fog->SetActorLocation(m_fog->GetActorLocation() + FVector(0.0f, 0.0f, 300.0f));

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}


	// Init PixelShading and ComputeShading
	PixelShading = new FPixelShaderUsageExample(PixelShaderTopLeftColor, GetWorld()->Scene->GetFeatureLevel());
	ComputeShading = new FComputeShaderUsageExample(ComputeShaderSimulationSpeed, 1024, 1024, GetWorld()->Scene->GetFeatureLevel());

}

void APaintballFarniaCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	if (PixelShading) {
		delete PixelShading;
	}
	if (ComputeShading) {
		delete ComputeShading;
	}
}

void APaintballFarniaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Loc = this->GetActorLocation();

	m_fog->revealSmoothCircle(FVector2D(Loc.X, Loc.Y), fowRadiusReveal);

	// Add current to elapsed time
	TotalElapsedTime += DeltaTime;
	if (PixelShading) {
		EndColorBuildup = FMath::Clamp(EndColorBuildup + DeltaTime * EndColorBuildupDirection, 0.0f, 1.0f);
		if (EndColorBuildup >= 1.0 || EndColorBuildup <= 0) {
			EndColorBuildupDirection *= -1;
		}
		FTexture2DRHIRef InputTexture = NULL;
		if (ComputeShading) {
			ComputeShading->ExecuteComputeShader(TotalElapsedTime);
			InputTexture = ComputeShading->GetTexture(); //This is the output texture from the compute shader that we will pass to the pixel shader. 
		}
		ComputeShaderBlend = FMath::Clamp(ComputeShaderBlend + ComputeShaderBlendScalar * DeltaTime, 0.0f, 1.0f);
		PixelShading->ExecutePixelShader(RenderTarget, InputTexture, FColor(EndColorBuildup * 255, 0, 0, 255), ComputeShaderBlend);
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void APaintballFarniaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APaintballFarniaCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APaintballFarniaCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APaintballFarniaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APaintballFarniaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APaintballFarniaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APaintballFarniaCharacter::LookUpAtRate);

	//ShaderPluginDemo Specific input mappings  
	InputComponent->BindAction("SavePixelShaderOutput", IE_Pressed, this, &APaintballFarniaCharacter::SavePixelShaderOutput);
	InputComponent->BindAction("SaveComputeShaderOutput", IE_Pressed, this, &APaintballFarniaCharacter::SaveComputeShaderOutput);
	InputComponent->BindAxis("ComputeShaderBlend", this, &APaintballFarniaCharacter::ModifyComputeShaderBlend);

}

void APaintballFarniaCharacter::OnFire()
{
	// OG Logic
	/*
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<APaintballFarniaProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<APaintballFarniaProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	*/

	// 5-2 Logic
	/*
	// Start a trace with distance 10,000
	FHitResult HitResult;
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FRotator Direction = FirstPersonCameraComponent->GetComponentRotation();
	FVector EndLocation = StartLocation + Direction.Vector() * 10000;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	// Do the trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams)) {
		// Find hit meshes and actors
		TArray <UStaticMeshComponent*> StaticMeshComponents = TArray <UStaticMeshComponent*>();
		AActor* HitActor = HitResult.GetActor();
		// Check if hit was valid
		if (NULL != HitActor) {
			// Get hit meshes
			HitActor->GetComponents <UStaticMeshComponent>(StaticMeshComponents);
			for (int32 i = 0; i < StaticMeshComponents.Num(); i++) {
				// Change hit mesh to our material
				UStaticMeshComponent * CurrentStaticMeshPtr = StaticMeshComponents[i];
				CurrentStaticMeshPtr->SetMaterial(0, MaterialToApplyToClickedObject);
				UMaterialInstanceDynamic* MID = CurrentStaticMeshPtr->CreateAndSetMaterialInstanceDynamic(0);
				UTexture* CastedRenderTarget = Cast <UTexture>(RenderTarget);
				MID->SetTextureParameterValue("InputTexture", CastedRenderTarget);
			}
		}
	}
	*/

	// 5-3 Logic
	// Create blank hit
	FHitResult HitResult;

	// Set start location and rotation
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FRotator Direction = FirstPersonCameraComponent->GetComponentRotation();

	// Add together for end location
	FVector EndLocation = StartLocation + Direction.Vector() * 10000;

	// Ignore casting actor
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Start Trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams)) {

		// Cast hit actor for interface
		IGlowInterface* pointerToInterface = Cast<IGlowInterface>(HitResult.GetActor());
		if (pointerToInterface != nullptr)
		{
			// Execute command if interface found
			pointerToInterface->Execute_TriggerGlow(HitResult.GetActor());
		}

		// Debug Interface
		else
		{
			FString actorName = UKismetSystemLibrary::GetDisplayName(HitResult.GetActor());
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast to interface on actor: %s"), *actorName);
		}
	}
	// Debug Hit
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didnt hit anything"));
	}
}

void APaintballFarniaCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APaintballFarniaCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void APaintballFarniaCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void APaintballFarniaCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void APaintballFarniaCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APaintballFarniaCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APaintballFarniaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APaintballFarniaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool APaintballFarniaCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APaintballFarniaCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &APaintballFarniaCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APaintballFarniaCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void APaintballFarniaCharacter::ModifyComputeShaderBlend(float NewScalar)
{
	ComputeShaderBlendScalar = NewScalar;
}

void APaintballFarniaCharacter::SavePixelShaderOutput()
{
	PixelShading->Save();
}

void APaintballFarniaCharacter::SaveComputeShaderOutput()
{
	ComputeShading->Save();
}
