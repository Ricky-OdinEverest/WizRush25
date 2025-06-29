// Fill out your copyright notice in the Description page of Project Settings.


#include "WAction_ProjectileAttack.h"

#include "Combat/WDashComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UWAction_ProjectileAttack::UWAction_ProjectileAttack()
{
	AttackAnimDelay = 0.02f;
	//TimeToHitParamName = "TimeToHit";
	HandSocketName = "hand_r";
	//CharacterRef = GetOwner<ACharacter>();
	
	//Controller = GetWorld()->GetFirstPlayerController();
	//MovementComp = CharacterRef->GetCharacterMovement();
	//DashComponent = CharacterRef->FindComponentByClass<UWDashComponent>();

	//bIsAiming = false;
	//bIsShooting = false;
}

void UWAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		RotateCharacterToMouseCursor(Character);
		StartAiming(Character);
	}
	
}



void UWAction_ProjectileAttack::StartAiming(ACharacter* InstigatorCharacter)
{
	//if (!CharacterRef || !MovementComp) return;

	// Check if dashing
	//if (DashComponent && DashComponent->IsDashing())
	//{
	//	return; // Prevent aiming while dashing
	//}
	
	//bIsAiming = true;
	InstigatorCharacter-> GetCharacterMovement()->bOrientRotationToMovement = false;  // Disable rotation to movement
	
	InstigatorCharacter->bUseControllerRotationYaw = true;  // Use controller rotation when aiming
	
	// Set the character's speed to 300 while aiming
	InstigatorCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	
	StartRepeatingAttack(InstigatorCharacter);  // Start the repeating attack
}



void UWAction_ProjectileAttack::RotateCharacterToMouseCursor(ACharacter* InstigatorCharacter)
{
	APlayerController*  PlayerController = Cast<APlayerController>(InstigatorCharacter->GetController());
	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FVector MouseLocation, MouseDirection;
		if (PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
		{
			// Get the character's location
			FVector CharacterLocation = InstigatorCharacter->GetActorLocation();
            
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
			InstigatorCharacter->SetActorRotation(TargetRotation);
			
			// Store the calculated mouse direction to use later in firing
			CachedMouseDirection = DirectionToMouse;
		}
	}
}

void UWAction_ProjectileAttack::StartRepeatingAttack(ACharacter* InstigatorCharacter)
{
	// Start a looping timer that calls PrimaryAttack_TimeElapsed every 0.5 seconds
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "PrimaryAttack_TimeElapsed", InstigatorCharacter);
	InstigatorCharacter->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, 0.2f, true);
	//bIsShooting = true;
}



void UWAction_ProjectileAttack::PrimaryAttack_TimeElapsed(ACharacter* InstigatorCharacter)
{
	FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("hand_r");
	
	// Use the cached mouse direction calculated in RotateCharacterToMouseCursor
	FVector AimDirection = CachedMouseDirection;
     
	// Calculate rotation from aim direction
	FRotator AimRotation = FRotationMatrix::MakeFromX(AimDirection).Rotator();
    
	FTransform SpawnTM = FTransform(AimRotation, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void UWAction_ProjectileAttack::StopAction_Implementation(AActor* Instigator)
{
	
	Super::StopAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		StopAiming(Character);
		StopRepeatingAttack(Character);
	}
	

}


void UWAction_ProjectileAttack::StopRepeatingAttack(ACharacter* InstigatorCharacter)
{
	// Clear the repeating attack timer
	InstigatorCharacter->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AttackDelay);
	//StopAction(InstigatorCharacter);
	//bIsShooting = false;
}

void UWAction_ProjectileAttack::StopAiming(ACharacter* InstigatorCharacter)
{
	//bIsAiming = false;

	InstigatorCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;  // Enable rotation to movement
	InstigatorCharacter->bUseControllerRotationYaw = false;  // Disable controller rotation when not aiming

	//StopRepeatingAttack(InstigatorCharacter);
}



