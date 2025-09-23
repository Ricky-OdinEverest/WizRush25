// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WDashComponent.h"

#include "Combat/TeleportMarker.h"
#include "Combat/WProjectileCombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UWDashComponent::UWDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UWDashComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
	Controller = GetWorld()->GetFirstPlayerController();
	
	MovementComp = CharacterRef->GetCharacterMovement();
	
	ProjectileCombatComponent = CharacterRef->FindComponentByClass<UWProjectileCombatComponent>();

	bIsDashing = false; // Initialize the Dash flag to false
	TeleportMarkerInstance = nullptr;
	
}


// Called every frame
void UWDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update DashChargeTime if dashing
	if (bIsDashing)
	{
		DashChargeTime += DeltaTime;  // Increment the charge time
		// Calculate current teleport distance
		float TeleportDistanceMultiplier = 2500.0f; // Ensure this matches your desired rate
		float TotalTeleportDistance = DashChargeTime * TeleportDistanceMultiplier;
		float TeleportDistance = FMath::Clamp(TotalTeleportDistance, 300.0f, MaxDashDistance);

		// Determine the teleport target location based on current forward direction
		FVector ForwardDirection = CharacterRef->GetActorForwardVector().GetSafeNormal();
		FVector StartLocation = CharacterRef->GetActorLocation();
		FVector TargetLocation = StartLocation + (ForwardDirection * TeleportDistance);
		// Update the TeleportMarker's position
		if (TeleportMarkerInstance)
		{
			TeleportMarkerInstance->SetActorLocation(TargetLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TeleportMarkerInstance is null during dash charge."));
		}
	}
}


void UWDashComponent::DashCharge()
{
	
	// Disable aiming if currently aiming
	if (ProjectileCombatComponent && ProjectileCombatComponent->IsAiming())
	{
		ProjectileCombatComponent->StopAiming();
	}
	// Disable aiming if currently aiming
	if (ProjectileCombatComponent && ProjectileCombatComponent->IsShooting())
	{
		ProjectileCombatComponent->StopRepeatingAttack();
	}
	
	bIsDashing = true;  // Start dash state
     
	// Stop movement but allow rotation
	MovementComp->MaxWalkSpeed = 0.0f;
	// Increase the rotation rate for faster rotation during dash
	MovementComp->RotationRate = FRotator(0.0f, 10000.0f, 0.0f);  // Higher value for faster rotation
	
	CharacterRef->bUseControllerRotationYaw = false; // Reset controller rotation
	// Spawn the TeleportMarker at the current location
	if (TeleportMarkerClass && !TeleportMarkerInstance)
	{
		FVector CurrentLocation = CharacterRef->GetActorLocation();
		FRotator CurrentRotation = CharacterRef->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = CharacterRef;
		SpawnParams.Instigator = CharacterRef->GetInstigator();

		TeleportMarkerInstance = GetWorld()->SpawnActor<ATeleportMarker>(TeleportMarkerClass, CurrentLocation, CurrentRotation, SpawnParams);
		if (TeleportMarkerInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("TeleportMarker spawned at: %s"), *CurrentLocation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn TeleportMarker."));
		}
	}
}

void UWDashComponent::DashInitiate()
{
	
	bIsDashing = false;  // End dashCharge state
	// Calculate teleport distance based on DashChargeTime
	float TeleportDistanceMultiplier = 2500.0f; // Adjust this to control how fast teleport distance increases
	float TotalTeleportDistance = DashChargeTime * TeleportDistanceMultiplier;
	float TeleportDistance = FMath::Clamp(TotalTeleportDistance, 1000.0f, MaxDashDistance);  // Adjust min and max values as needed
    
	// Determine the teleport target location
	FVector ForwardDirection = CharacterRef->GetActorForwardVector().GetSafeNormal();
	FVector StartLocation = CharacterRef->GetActorLocation();
	FVector TargetLocation = StartLocation + (ForwardDirection * TeleportDistance);
    
	UE_LOG(LogTemp, Warning, TEXT("DashChargeTime: %f, TotalTeleportDistance: %f, ClampedTeleportDistance: %f"),
			  DashChargeTime, TotalTeleportDistance, TeleportDistance);

	// Teleport the character
	//bool bTeleportSuccess = CharacterRef->SetActorLocation(TargetLocation, false);  // Sweep for collision
	
	bool bTeleportSuccess = CharacterRef->TeleportTo(TargetLocation, CharacterRef->GetActorRotation());

	if (!bTeleportSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Teleportation failed due to collision."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Teleported to: %s"), *TargetLocation.ToString());
	}

	// Destroy the TeleportMarker
	if (TeleportMarkerInstance)
	{
		TeleportMarkerInstance->Destroy();
		TeleportMarkerInstance = nullptr;
	}

	// Reset DashChargeTime after teleporting
	DashChargeTime = 0.0f;

	// Reset movement properties
	MovementComp->MaxWalkSpeed = 600.0f;  // Reset speed
	MovementComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // Lower Rotation Speed

	// Reset DashChargeTime after teleporting
	DashChargeTime = 0.0f;
     
     
	MovementComp->MaxWalkSpeed = 600.0f;  // Reset speed
	MovementComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f); //Lower Rotation Speed
}
