// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeAutoCloseComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/EnemyInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMeleeAutoCloseComponent::UMeleeAutoCloseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeAutoCloseComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
}

void UMeleeAutoCloseComponent::StartLockon( double BoxCollisionLength, double BoxCollisionWidth, double BoxCollisionHeight  )
{
	FHitResult OutResult;
	FVector StartLocation{ GetOwner()->GetActorLocation()};
	FVector ForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector EndLocation = StartLocation + (ForwardDirection * BoxCollisionLength); // Move forward

	double SweepDistance {FVector::Distance(StartLocation, EndLocation)};
	
	FVector BoxHalfExtent = FVector(SweepDistance / 2.0f, BoxCollisionWidth / 2.0f, BoxCollisionHeight / 2.0f); // Half of Length, Width, Height

	// Shift the sweep forward to prevent overlapping behind the player
	FVector AdjustedStart = StartLocation + (ForwardDirection * BoxHalfExtent.X);
	FVector AdjustedEnd = EndLocation - (ForwardDirection * BoxHalfExtent.X);
	
	// Define the box shape
	FCollisionShape Box{
		FCollisionShape::MakeBox(BoxHalfExtent)
	};

	// Calculate the box's rotation based on the player's forward direction
	FQuat ShapeRotation = GetOwner()->GetActorQuat(); // Rotates the box to align with the player

	
	FCollisionQueryParams IgnoreParams{
	FName{ TEXT("Ignore Collision Params")},
		false,
		GetOwner()
	};
	
	
	bool bHasFoundTarget {GetWorld()->SweepSingleByChannel(
		OutResult,
		AdjustedStart ,
		AdjustedEnd,
		ShapeRotation,
		ECC_GameTraceChannel1,
		Box,
		IgnoreParams
		)};

	// Draw the debug box exactly as it is being swept
	FVector BoxCenter{
		UKismetMathLibrary::VLerp(
			StartLocation,EndLocation,0.5f
			)
	}; // Get the midpoint of the sweep

	// Draw the debug box at the midpoint of the sweep (which covers the entire sweep path)
if (DebugWarpBox)
{
	UKismetSystemLibrary::DrawDebugBox(
	GetWorld(),
	BoxCenter,
	Box.GetExtent(),
	bHasFoundTarget ? FColor::Green : FColor::Red,
	ShapeRotation.Rotator(),
2.0f,
2.0f
	);
}

	
	if (!bHasFoundTarget) {return;}

	if (!OutResult.GetActor()->Implements<UEnemyInterface>()) { return; }
	CurrentTargetActor = OutResult.GetActor();

	// Lock or Unlocks looks input
	//Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	OwnerRef->bUseControllerRotationYaw = true;
	//MovementComp->bUseControllerDesiredRotation = true;

	//Ground Friction does not appear to interfere with the player SlideTowardsTarget function
	//MovementComp->GroundFriction = 0.0f;

	IEnemyInterface::Execute_OnSelect(CurrentTargetActor);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);


	// Start the sliding motion towards the target
	//SlideTowardsTarget();
	
	
}

void UMeleeAutoCloseComponent::StartLockonMotionWarping(double BoxCollisionLength, double BoxCollisionWidth,
	double BoxCollisionHeight)
{
	FHitResult OutResult;
	FVector StartLocation{ GetOwner()->GetActorLocation()};
	FVector ForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector EndLocation = StartLocation + (ForwardDirection * BoxCollisionLength); // Move forward

	double SweepDistance {FVector::Distance(StartLocation, EndLocation)};
	
	FVector BoxHalfExtent = FVector(SweepDistance / 2.0f, BoxCollisionWidth / 2.0f, BoxCollisionHeight / 2.0f); // Half of Length, Width, Height

	// Shift the sweep forward to prevent overlapping behind the player
	FVector AdjustedStart = StartLocation + (ForwardDirection * BoxHalfExtent.X);
	FVector AdjustedEnd = EndLocation - (ForwardDirection * BoxHalfExtent.X);
	
	// Define the box shape
	FCollisionShape Box{
		FCollisionShape::MakeBox(BoxHalfExtent)
	};

	// Calculate the box's rotation based on the player's forward direction
	FQuat ShapeRotation = GetOwner()->GetActorQuat(); // Rotates the box to align with the player

	
	FCollisionQueryParams IgnoreParams{
	FName{ TEXT("Ignore Collision Params")},
		false,
		GetOwner()
	};
	
	
	bool bHasFoundTarget {GetWorld()->SweepSingleByChannel(
		OutResult,
		AdjustedStart ,
		AdjustedEnd,
		ShapeRotation,
		ECC_GameTraceChannel1,
		Box,
		IgnoreParams
		)};

	// Draw the debug box exactly as it is being swept
	FVector BoxCenter{
		UKismetMathLibrary::VLerp(
			StartLocation,EndLocation,0.5f
			)
	}; // Get the midpoint of the sweep

	// Draw the debug box at the midpoint of the sweep (which covers the entire sweep path)

	if (DebugWarpBox)
	{
		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			BoxCenter,
			Box.GetExtent(),
			bHasFoundTarget ? FColor::Green : FColor::Red,
			ShapeRotation.Rotator(),
	2.0f,
	2.0f
			);
	}
	
	if (!bHasFoundTarget) {return;}

	if (!OutResult.GetActor()->Implements<UEnemyInterface>()) { return; }
	CurrentTargetActor = OutResult.GetActor();

	// Lock or Unlocks looks input
	//Controller->SetIgnoreLookInput(true);
	//MovementComp->bOrientRotationToMovement = false;
	//OwnerRef->bUseControllerRotationYaw = true;
	//MovementComp->bUseControllerDesiredRotation = true;



	IEnemyInterface::Execute_OnSelect(CurrentTargetActor);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	
	
	
}

void UMeleeAutoCloseComponent::ToggleLockon(double BoxCollisionLength, double BoxCollisionWidth,
                                            double BoxCollisionHeight)
{
	if(IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(BoxCollisionLength, BoxCollisionWidth, BoxCollisionHeight);
	}
	
}

void UMeleeAutoCloseComponent::ToggleLockonMotionWarping(double BoxCollisionLength, double BoxCollisionWidth,
	double BoxCollisionHeight)
{
	if(IsValid(CurrentTargetActor))
	{
		// Since the Tick Component is Checking for CurrentTargetActor
		CurrentTargetActor = nullptr;
	}
	else
	{
		StartLockonMotionWarping(BoxCollisionLength, BoxCollisionWidth, BoxCollisionHeight);
	}
}

void UMeleeAutoCloseComponent::EndLockon()
{
	// not yet implemented
	if (IsValid(CurrentTargetActor))
	{
		IEnemyInterface::Execute_OnDeSelect(CurrentTargetActor);
	}
	// Since the Tick Component is Checking for CurrentTargetActor
	CurrentTargetActor = nullptr;
	// Overrides bUseControllerDesiredRotation
	MovementComp->bOrientRotationToMovement = true;
	OwnerRef->bUseControllerRotationYaw = false;

	// This may have more to do with how the camera is effected so I can get rid of it in the final build or ignore it
	//Controller->ResetIgnoreLookInput();

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

// Called every frame
void UMeleeAutoCloseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentTargetActor)) { return; }


}

// This function handles the sliding towards the target
void UMeleeAutoCloseComponent::SlideTowardsTarget()
{
	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	// Calculate the distance to the target
	float CurrentDistance = FVector::Distance(CurrentLocation, TargetLocation);
	float DesiredDistance = 200.0f;  // This can be adjustable as needed

	// Move towards the target if farther than desired distance
	if (CurrentDistance > DesiredDistance)
	{
		// Calculate the direction towards the target
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

		// Set the speed of the sliding
		float SlideSpeed = 1000.0f;  // Speed at which the player slides

		// Calculate how much to move this frame
		FVector MoveAmount = Direction * SlideSpeed * GetWorld()->GetDeltaSeconds();  // DeltaTime-based movement

		// Check if moving this frame will overshoot the desired position
		if (MoveAmount.Size() > (CurrentDistance - DesiredDistance))
		{
			// Snap to the desired position to avoid overshooting
			MoveAmount = Direction * (CurrentDistance - DesiredDistance);
		}

		// Move the player
		OwnerRef->AddActorWorldOffset(MoveAmount, true);  // true ensures that collision is handled
	}
}

FVector UMeleeAutoCloseComponent::GetTranslationWarpTarget()
{
	if (CurrentTargetActor == nullptr) return FVector();

	const FVector CombatTargetLocation = CurrentTargetActor->GetActorLocation();
	const FVector Location = OwnerRef->GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;

}

FVector UMeleeAutoCloseComponent::GetRotationWarpTarget()
{
	if (CurrentTargetActor)
	{
		return CurrentTargetActor->GetActorLocation();
	}
	return FVector();
}
