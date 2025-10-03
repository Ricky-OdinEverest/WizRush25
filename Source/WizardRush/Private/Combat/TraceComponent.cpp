// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"

#include "CombatInterface.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(false);
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()
		->FindComponentByClass<USkeletalMeshComponent>();
}



// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) { return; }
	
	switch (TraceShape)
	{
	case ETraceShape::Box:
		if (SocketSweepTrace()) return;
		break;

	case ETraceShape::Capsule:
		if (CapsuleSweepTrace()) return;
		break;
	}
}

void UTraceComponent::HandleResetAttack()
{
	
}

void UTraceComponent::StartTraceWindow()
{
	TargetsToIgnore.Empty();
	SetComponentTickEnabled(true);
	bIsAttacking = true; 
}

void UTraceComponent::EndTraceWindow()
{
	SetComponentTickEnabled(false);
	bIsAttacking = false; 
	TargetsToIgnore.Empty();
}

void UTraceComponent::SetTraceParams(ETraceShape InTraceShape, bool debug)
{
	TraceShape = InTraceShape;
	bDebugMode = debug;
}



bool UTraceComponent::SocketSweepTrace()
{
	TArray<FHitResult> AllResults;

	for (const FTraceSockets Socket: Sockets) {
		FVector StartSocketLocation{ 
			SkeletalComp->GetSocketLocation(Socket.Start) 
		};
		FVector EndSocketLocation{ 
			SkeletalComp->GetSocketLocation(Socket.End) 
		};
		FQuat ShapeRotation{ 
			SkeletalComp->GetSocketQuaternion(Socket.Rotation) 
		};

		TArray<FHitResult> OutResults;
		double WeaponDistance{
			FVector::Distance(StartSocketLocation, EndSocketLocation)
		};
		FVector BoxHalfExtent{
			BoxCollisionLength, BoxCollisionLength, WeaponDistance
		};
		BoxHalfExtent /= 2; // BoxHalfExtent = BoxHalfExtent / 2;
		FCollisionShape Box{
			FCollisionShape::MakeBox(BoxHalfExtent)
		};
		FCollisionQueryParams IgnoreParams{
			FName { TEXT("Ignore Params") },
			false,
			GetOwner()
		};

		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Box,
			IgnoreParams
		) };

		for (FHitResult Hit: OutResults) {
			AllResults.Add(Hit);
		}

		if (bDebugMode)
		{
			FVector CenterPoint{
				UKismetMathLibrary::VLerp(
					StartSocketLocation, EndSocketLocation, 0.5f
				)
			};

			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				Box.GetExtent(),
				bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
				ShapeRotation.Rotator(),
				1.0f,
				2.0f
			);
		}
	}


	if (AllResults.Num() == 0) { return true; }

	float CharacterDamage{ 0.0f };

	ICombatInterface* FighterRef{ Cast<ICombatInterface>(GetOwner()) };

	if (FighterRef)
	{ 
		CharacterDamage = FighterRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;
	

	for (const FHitResult& Hit: AllResults)
	{
		AActor* TargetActor{ Hit.GetActor() };

		if (TargetsToIgnore.Contains(TargetActor)) { continue; }

		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()  
		);

		TargetsToIgnore.AddUnique(TargetActor);
	}
	return false;
}
bool UTraceComponent::CapsuleSweepTrace()
{
	UCapsuleComponent* Capsule = GetOwner()->FindComponentByClass<UCapsuleComponent>();
	if (!Capsule) return false;

	float Radius = Capsule->GetScaledCapsuleRadius();
	float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();

	// Default sweep: from actor location forward
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * 10.f; // Sweep forward 100 units

	TArray<FHitResult> OutResults;

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	FCollisionQueryParams IgnoreParams(FName("CapsuleTraceIgnore"), false, GetOwner());
	IgnoreParams.AddIgnoredActors(TargetsToIgnore);

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		CapsuleShape,
		IgnoreParams
	);

	if (bDebugMode)
	{
		FVector Center = UKismetMathLibrary::VLerp(Start, End, 0.5f);
		UKismetSystemLibrary::DrawDebugCapsule(
			GetWorld(),
			Center,
			HalfHeight,
			Radius,
			GetOwner()->GetActorRotation(),
			bHit ? FLinearColor::Green : FLinearColor::Red,
			1.0f,
			2.0f
		);
	}

	if (!bHit) return true;

	float CharacterDamage = 0.0f;
	if (ICombatInterface* FighterRef = Cast<ICombatInterface>(GetOwner()))
	{
		CharacterDamage = FighterRef->GetDamage();
	}

	FDamageEvent DamageEvent;
	for (const FHitResult& Hit : OutResults)
	{
		AActor* Target = Hit.GetActor();
		if (!Target || TargetsToIgnore.Contains(Target)) continue;

		Target->TakeDamage(
			CharacterDamage,
			DamageEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);

		TargetsToIgnore.AddUnique(Target);
	}

	return false;
}
