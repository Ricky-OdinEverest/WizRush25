// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WProjectileCombatComponent.h"

#include "WProtagWizard.h"
#include "Combat/WDashComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class AWProtagWizard;
// Sets default values for this component's properties
UWProjectileCombatComponent::UWProjectileCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWProjectileCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterRef = GetOwner<ACharacter>();
	
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = CharacterRef->GetCharacterMovement();
	DashComponent = CharacterRef->FindComponentByClass<UWDashComponent>();

	bIsAiming = false;
	bIsShooting = false;
	
	
}


// Called every frame
void UWProjectileCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Rotate character to face mouse when aiming
	if (bIsAiming)
	{
		RotateCharacterToMouseCursor();
	}
	
}

// Create the Aim functions
void UWProjectileCombatComponent::StartAiming()
{

	if (!CharacterRef || !MovementComp) return;

	// Check if dashing
	if (DashComponent && DashComponent->IsDashing())
	{
		return; // Prevent aiming while dashing
	}
	
	   bIsAiming = true;
	
		CharacterRef-> GetCharacterMovement()->bOrientRotationToMovement = false;  // Disable rotation to movement
	
		CharacterRef->bUseControllerRotationYaw = true;  // Use controller rotation when aiming
	
		// Set the character's speed to 300 while aiming
		MovementComp->MaxWalkSpeed = 300.0f;
	
		StartRepeatingAttack();  // Start the repeating attack
	
}

void UWProjectileCombatComponent::StopAiming()
{
	bIsAiming = false;

	MovementComp->bOrientRotationToMovement = true;  // Enable rotation to movement
	CharacterRef->bUseControllerRotationYaw = false;  // Disable controller rotation when not aiming
	
	// Revert the character's speed back to 600 when not aiming
	// Check if not dashing before resetting speed
	if (DashComponent && !DashComponent->IsDashing())
	{
		MovementComp->MaxWalkSpeed = 600.0f;
	}

	StopRepeatingAttack();
}


// Function to start the repeating attack
void UWProjectileCombatComponent::StartRepeatingAttack()
{
	// Start a looping timer that calls PrimaryAttack_TimeElapsed every 0.5 seconds
	CharacterRef->GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &UWProjectileCombatComponent::PrimaryAttack_TimeElapsed, 0.2f, true);
	bIsShooting = true;
}


// Function to stop the repeating attack
void UWProjectileCombatComponent::StopRepeatingAttack()
{
	// Clear the repeating attack timer
	CharacterRef->GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	bIsShooting = false;
}

// Calculate the new rotation to face the mouse cursor
void UWProjectileCombatComponent::RotateCharacterToMouseCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FVector MouseLocation, MouseDirection;
		if (PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
		{
			// Get the character's location
			FVector CharacterLocation = CharacterRef->GetActorLocation();
            
			// Calculate the intersection point of the mouse ray and a plane at the character's Z level
			float ZPlane = CharacterLocation.Z;
			FVector PlanePoint = FVector(MouseLocation.X, MouseLocation.Y, ZPlane);
            
			// Calculate the intersection point by projecting the mouse direction to the plane at ZPlane
			float T = (ZPlane - MouseLocation.Z) / MouseDirection.Z;
			FVector ProjectedMouseLocation = MouseLocation + T * MouseDirection;

			// Calculate direction to the projected mouse location
			FVector DirectionToMouse = (ProjectedMouseLocation - CharacterLocation).GetSafeNormal();

			// Create a new yaw rotation from the direction
			FRotator TargetRotation = FRotator(0.0f, DirectionToMouse.Rotation().Yaw, 0.0f);

			// Set the character's rotation directly to face the mouse
			CharacterRef->SetActorRotation(TargetRotation);
			
			// Store the calculated mouse direction to use later in firing
			CachedMouseDirection = DirectionToMouse;
		}
	}
}

void UWProjectileCombatComponent::PrimaryAttack()
{

	//PlayAnimMontage(AttackAnim);

	CharacterRef->GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &UWProjectileCombatComponent::PrimaryAttack_TimeElapsed, 0.2f);

	//potential to clear timer or interupt animation if charachter dies
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);

}

void UWProjectileCombatComponent:: PrimaryAttack_TimeElapsed()
{

	FVector HandLocation = CharacterRef->GetMesh()->GetSocketLocation("hand_r");
	
	// Use the cached mouse direction calculated in RotateCharacterToMouseCursor
	FVector AimDirection = CachedMouseDirection;
     
	// Calculate rotation from aim direction
	FRotator AimRotation = FRotationMatrix::MakeFromX(AimDirection).Rotator();
    
	FTransform SpawnTM = FTransform(AimRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = CharacterRef;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}
