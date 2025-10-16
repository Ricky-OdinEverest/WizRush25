// Fill out your copyright notice in the Description page of Project Settings.

#include "WProtagWizard.h"

#include "WActionComponent.h"
#include "Animations/WizPlayeryAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Combat/MeleeAutoCloseComponent.h"
#include "Combat/MeleeCombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/WDashComponent.h"
#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Combat/WProjectileCombatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Stats/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AWProtagWizard::AWProtagWizard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	


	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComp->SetupAttachment(SpringArmComponent);

	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	MeleeAutoCloseComp = CreateDefaultSubobject<UMeleeAutoCloseComponent>(TEXT("Auto Close Component"));
	MeleeCombatComp = CreateDefaultSubobject<UMeleeCombatComponent>(TEXT("Melee Combat Component"));
	
	ProjectileCombatComp = CreateDefaultSubobject<UWProjectileCombatComponent>(TEXT("Projectile Combat Component"));
	
	DashComponent = CreateDefaultSubobject<UWDashComponent>(TEXT("Dash Component"));

	ActionComp = CreateDefaultSubobject<UWActionComponent>(TEXT("Action Component"));

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	PlayerMotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));


	// Shield Collision Sphere
	ShieldCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollisionSphere"));
	ShieldCollisionSphere->SetupAttachment(RootComponent);
	ShieldCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldCollisionSphere->InitSphereRadius(50.f);

	//Debug
	ShieldCollisionSphere->SetHiddenInGame(false);
	ShieldCollisionSphere->SetVisibility(true);
	
}

UMotionWarpingComponent* AWProtagWizard::GetMotionWarpingComponent() const
{
	return PlayerMotionWarpingComponent;
}

// Called when the game starts or when spawned
void AWProtagWizard::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UWizPlayerAnimInstance>(
		GetMesh()->GetAnimInstance()
		);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;  // Show mouse cursor
		/*UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}*/
	}
	
}


void AWProtagWizard::MoveForward(float Value)
{
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.0f;
    ControlRot.Yaw = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void AWProtagWizard::MoveRight(float Value)
{
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
    ControlRot.Yaw = 0.0f;
	
	
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AWProtagWizard::SprintStart()
{
	ActionComp->StartActionByName(this, "sprint");
}

void AWProtagWizard::SprintStop()
{
	ActionComp->StopActionByName(this, "sprint");
}

void AWProtagWizard::PrimaryAttackStart()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AWProtagWizard::PrimaryAttackStop()
{
	ActionComp->StopActionByName(this, "PrimaryAttack");
}


void AWProtagWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void AWProtagWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AWProtagWizard::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWProtagWizard::SprintStop);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AWProtagWizard::SprintStop);
	}*/


	PlayerInputComponent->BindAxis("MoveForward", this, &AWProtagWizard::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWProtagWizard::MoveRight);

	
	

	// Ensure ProjectileCombatComponent is valid
	if (ProjectileCombatComp)
	{
		PlayerInputComponent->BindAction("Aim", IE_Pressed, ProjectileCombatComp, &UWProjectileCombatComponent::StartAiming);
		PlayerInputComponent->BindAction("Aim", IE_Released, ProjectileCombatComp, &UWProjectileCombatComponent::StopAiming);
		//PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, ProjectileCombatComp, &UWProjectileCombatComponent::PrimaryAttack);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileCombatComponent is null in SetupPlayerInputComponent"));
	}

	if(DashComponent)
	{
		// Bind the DashCharge and DashInitiate functions to the input
		//PlayerInputComponent->BindAction("DashCharge", IE_Pressed, DashComponent, &UWDashComponent::DashCharge);
		//PlayerInputComponent->BindAction("DashCharge", IE_Released, DashComponent, &UWDashComponent::DashInitiate);
	}
	
	
}

float AWProtagWizard::GetDamage()
{
	return 10.f;
}

void AWProtagWizard::HandleDeath()
{
	PlayAnimMontage(PlayerDeathMontage);

	IsDead = true;

	DisableInput(GetController<APlayerController>());

}

void AWProtagWizard::MakeBlockCollision()
{
}

bool AWProtagWizard::BlockCheck(AActor* Opponent)
{
	/**ACharacter* CharacterRef{ GetOwner<ACharacter>() };

	if (!CharacterRef->Implements<UMainPlayer>()) { return true; }

	IMainPlayer* PlayerRef{ Cast<IMainPlayer>(CharacterRef) };
	
	FVector OpponentForward{ Opponent->GetActorForwardVector() };
	FVector PlayerForward{ CharacterRef->GetActorForwardVector() };

	double Result{ FVector::DotProduct(OpponentForward, PlayerForward) };

	if (Result > 0 || !PlayerRef->HasEnoughStamina(StaminaCost)) 
	{ 
		return true; 
	}*/

	return false;

}

bool AWProtagWizard::CanTakeDamage()
{
	if(bPostHitInvincible) return false;
	
	if (PlayerAnim->bIsBlocking) 
	{
		return false;
	}

	return true;

}

void AWProtagWizard::PlayHurtAnim()
{
	
	if(bPostHitInvincible)
	{

		return;
	}
	if (PlayerAnim->bIsBlocking) return;
	
	PlayAnimMontage(HitReactAnimMontage);

	bPostHitInvincible = true;
	
	ProjectileCombatComp->StopAiming();
	ProjectileCombatComp->StopRepeatingAttack();
	MeleeCombatComp->HandleResetAttack();
	MeleeCombatComp->HandleResetCombo();
	DashComponent->DashInterrupt();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}


FRotator AWProtagWizard::GetMeleeRotationWarpTarget(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const
{
	const UCharacterMovementComponent* CMC = GetCharacterMovement();
	if (!CMC)
	{
		DeltaRotation = FRotator::ZeroRotator;
		return CurrentRotation;
	}
	
	FVector Direction = CMC->GetCurrentAcceleration();
	
	if (Direction.SizeSquared() < UE_KINDA_SMALL_NUMBER)
	{
		// AI path following request can orient us in that direction (it's effectively an acceleration)
		/*if (CMC->bHasRequestedVelocity && GetCharacterMovement()->RequestedVelocity.SizeSquared() > UE_KINDA_SMALL_NUMBER)
		{
			return GetCharacterMovement()->RequestedVelocity.GetSafeNormal().Rotation();
		}*/

		// Don't change rotation if there is no acceleration.
		return CurrentRotation;
	}

	// Rotate toward direction of acceleration.
	return Direction.Rotation();
}

void AWProtagWizard::GetHitMelee(const FVector& ImpactPoint, const AActor* Othercomp, bool Knockdown)
{
	// Spawn Niagara effect at impact point
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffect,
			ImpactPoint,
			FRotator::ZeroRotator
		);
	}

	// Play sound at location
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}