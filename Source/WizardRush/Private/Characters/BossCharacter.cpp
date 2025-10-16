// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "WProtagWizard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Stats/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

	

}

void ABossCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	EnemyAIController = Cast<AAIController>(NewController);
	if (EnemyAIController && BehaviorTree)
	{
		EnemyAIController->RunBehaviorTree(BehaviorTree);

		BlackboardComp = EnemyAIController->GetBlackboardComponent();

		if (BlackboardComp && BlackboardComp->GetKeyID(TEXT("CurrentState")) != FBlackboard::InvalidKey)
		{
			BlackboardComp->SetValueAsEnum(
				TEXT("CurrentState"),
				InitialState
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set CurrentState — invalid BlackboardComp or key"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PossessedBy failed — missing AIController or BehaviorTree"));
	}

	// Set default AI movement speed here
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = 800.f; // Adjust to desired value
	}
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AWProtagWizard* PlayerPawn = Cast<AWProtagWizard>(PC->GetPawn()))
		{
			if (PlayerPawn->StatsComp)
			{
				PlayerPawn->StatsComp->OnZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PlayerPawn->StatsComp is null in BossCharacter"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerPawn to AWProtagWizard in BossCharacter"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetFirstPlayerController returned null in BossCharacter"));
	}

	
}


// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	if (!BlackboardComp) return;

	EEnemyState CurrentState{
		static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")))
	};

	if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle)
	{
		return;
	}

	CombatTarget = DetectedPawn;

	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		EEnemyState::Range
	);

	if (BlackboardComp->GetKeyID(TEXT("TargetToFollow")) != FBlackboard::InvalidKey)
	{
		BlackboardComp->SetValueAsObject(
			TEXT("TargetToFollow"),
			PawnToDetect
		);

		UE_LOG(LogTemp, Log, TEXT("TargetToFollow set to %s"), *PawnToDetect->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetToFollow key not found in blackboard"));
	}


}

void ABossCharacter::GetHit(const FVector& ImpactPoint, const AActor* Othercomp)
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
void ABossCharacter::GetHitMelee(const FVector& ImpactPoint, const AActor* Othercomp, bool Knockdown)
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
	
	if (!Invincible)
	{
		EnemyAIController->GetBlackboardComponent()
		->SetValueAsEnum(
	TEXT("CurrentState"), EEnemyState::TakingDamage
	);
		
			EnemyAIController->GetBlackboardComponent()
			->SetValueAsEnum(
		TEXT("HandleDamage"), EHandleDamage::MeleeDamage
	);
		PlayHurtAnim(Knockdown);

		
	}


	

	//Invincible = true;
}
float ABossCharacter::GetDamage()
{
	return 10.f;
}

void ABossCharacter::HandlePlayerDeath()
{
	EnemyAIController->GetBlackboardComponent()
	->SetValueAsEnum(
		TEXT("CurrentState"), EEnemyState::GameOver
	);

}



void ABossCharacter::HandleDeath()
{
	float Duration{ PlayAnimMontage(DeathAnim) };

	EnemyAIController->GetBrainComponent()
		->StopLogic("defeated");

	FindComponentByClass<UCapsuleComponent>()
		->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABossCharacter::FinishDeathAnim,
		Duration,
		false
	);

}

void ABossCharacter::FinishDeathAnim()
{
	Destroy();
}

void ABossCharacter::PlayHurtAnim( bool Knockdown)
{
	if(Knockdown)
	{
		PlayAnimMontage(KnockdownAnim);
		bIsKnockedDown = true;
		Invincible = true;

		EnemyAIController->GetBlackboardComponent()
	   ->SetValueAsEnum(
	TEXT("CurrentState"), EEnemyState::Idle
);

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// Create a dynamic delegate for when montage ends
			FOnMontageEnded KnockedDownDelegate;
			KnockedDownDelegate.BindUObject(this, &ABossCharacter::GetUp);

			AnimInstance->Montage_SetEndDelegate(KnockedDownDelegate, KnockdownAnim);
		}

	}
	else
	{
		PlayAnimMontage(HitReactAnimMontage);
	}
	
}

void ABossCharacter::GetUp(UAnimMontage* Montage, bool bInterrupted)
{
	// When knockdown montage ends, play get-up animation
	if (Montage == KnockdownAnim && !bInterrupted)
	{
		bIsKnockedDown = false;
		/*PlayAnimMontage(GetUpAnim);
	
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance() )
        {
        	// Create a dynamic delegate for when montage ends
        	FOnMontageEnded GotUpDelegate;
        	GotUpDelegate.BindUObject(this, &ABossCharacter::RecoveredFromKnockdown);

        	AnimInstance->Montage_SetEndDelegate(GotUpDelegate, GetUpAnim);
        }*/
		
	}
}


void ABossCharacter::RecoveredFromKnockdown(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == GetUpAnim && !bInterrupted)
	{
		bIsKnockedDown = false;
	}
	
}