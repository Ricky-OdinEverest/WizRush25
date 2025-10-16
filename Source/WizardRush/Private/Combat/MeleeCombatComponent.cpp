// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeCombatComponent.h"

#include "CombatInterface.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMeleeCombatComponent::UMeleeCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	bIsMeleeAttacking = false;
	
}


// Called every frame
void UMeleeCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UMeleeCombatComponent::ComboAttack()
{

	if (!bCanMeleeAttack) {return;}
	bIsWindingUp = true;
	bIsMeleeAttacking = true;
	bCanMeleeAttack = false;
	
	CharacterRef->PlayAnimMontage(AttackAnimations[ComboCounter], 2.0f);

	ComboCounter++;

	int MaxCombo{ AttackAnimations.Num() };

	ComboCounter = UKismetMathLibrary::Wrap(
		ComboCounter,
		-1,
		(MaxCombo-1)
		);
}

void UMeleeCombatComponent::HandleResetAttack()
{
	bIsWindingUp = false;
	bCanMeleeAttack = true;
	bIsMeleeAttacking = false;
}

void UMeleeCombatComponent::HandleResetCombo()
{
	ComboCounter = 0;
}

void UMeleeCombatComponent::AttackTrace(bool Knockdown) const
{
	const FVector Start = CharacterRef->GetActorLocation();
	const FVector End = Start + (CharacterRef->GetActorForwardVector() * LengthFromChar);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(BoxX, BoxY, BoxZ),
		CharacterRef->GetActorRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())
	{

			float CharacterDamage{ 0.0f };

			ICombatInterface* Combatant{ Cast<ICombatInterface>(GetOwner()) };

			ICombatInterface* EnemyCombatant{ Cast<ICombatInterface>(BoxHit.GetActor()) };

			if (Combatant)
			{ 
				CharacterDamage = Combatant->GetDamage();
				
				if (EnemyCombatant)
				{
					if (Knockdown)
					{
						EnemyCombatant->GetHitMelee(BoxHit.GetActor()->GetActorLocation(), GetOwner(), true);
					}
					else
					{
						EnemyCombatant->GetHitMelee(BoxHit.GetActor()->GetActorLocation(), GetOwner(), false);
					}
					

				}

				FDamageEvent TargetAttackedEvent;

	
				AActor* TargetActor{ BoxHit.GetActor() };

				TargetActor->TakeDamage(
					CharacterDamage,
					TargetAttackedEvent,
					GetOwner()->GetInstigatorController(),
					GetOwner()  
				);

				FVector KnockbackDirection = (BoxHit.GetActor()->GetActorLocation() - CharacterRef->GetActorLocation()).GetSafeNormal2D();
				FVector NewLocation = BoxHit.GetActor()->GetActorLocation() + KnockbackDirection * 100.0f;

				BoxHit.GetActor()->SetActorLocation(NewLocation);


				// Rotate hit actor to face the player
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
					BoxHit.GetActor()->GetActorLocation(),
					CharacterRef->GetActorLocation()
				);

				// Only yaw rotation so enemies don’t tilt up/down
				LookAtRotation.Pitch = 0.f;
				LookAtRotation.Roll = 0.f;

				// Apply rotation and knockback
				BoxHit.GetActor()->SetActorRotation(LookAtRotation);
				BoxHit.GetActor()->SetActorLocation(NewLocation);
				
			}
		
	}
}

void UMeleeCombatComponent::WindUpOn() 
{
	bIsWindingUp = true;
}

void UMeleeCombatComponent::WindUpOff() 
{
	bIsWindingUp = false;
}


/*
IEnemyInterface* HitInterface = Cast<IEnemyInterface>(BoxHit.GetActor());
if (HitInterface)
{
	HitInterface->GetHit(BoxHit.ImpactPoint,CharacterRef );


	UPrimitiveComponent* MyComp    = Cast<UPrimitiveComponent>(CharacterRef->GetRootComponent());
	UPrimitiveComponent* OtherComp = BoxHit.Component.Get();
	bool bSelfMoved                = false;                      
	FVector NormalImpulse          = FVector::ZeroVector;        


	CharacterRef->NotifyHit(
	  MyComp,
	  CharacterRef,
	  OtherComp,
	  bSelfMoved,
	  BoxHit.ImpactPoint,
	  BoxHit.ImpactNormal,
	  NormalImpulse,
	  BoxHit
	);
	*/
