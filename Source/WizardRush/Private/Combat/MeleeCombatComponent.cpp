// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeCombatComponent.h"
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
	bCanMeleeAttack = true;
	bIsMeleeAttacking = false;
}

void UMeleeCombatComponent::AttackTrace() const
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
		IEnemyInterface* HitInterface = Cast<IEnemyInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint,CharacterRef );

			// grab the components we need
			UPrimitiveComponent* MyComp    = Cast<UPrimitiveComponent>(CharacterRef->GetRootComponent());
			UPrimitiveComponent* OtherComp = BoxHit.Component.Get();
			bool bSelfMoved                = false;                       // usually "false" for hits caused by others
			FVector NormalImpulse          = FVector::ZeroVector;         // you can compute a real impulse if you like

			// now call the engine’s NotifyHit to fire OnActorHit/ReceiveHit/Blueprint Hit events
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
		}
	}
	
	
	
}

