// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Stats/StatsComponent.h"

#include "CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	float HealthPercent = GetHealth(EStat::Health, EStat::MaxHealth);
	OnHealthPercentUpdateDelegate.Broadcast(HealthPercent);
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float Amount)
{
	if (Stats[EStat::Health] <= 0) { return; }

	ICombatInterface* FighterRef{ GetOwner<ICombatInterface>() };

	if (!FighterRef->CanTakeDamage()) { return; }


	Stats[EStat::Health] -= Amount;
	Stats[EStat::Health] = UKismetMathLibrary::FClamp(
		Stats[EStat::Health],
		0,
		Stats[EStat::MaxHealth]
	);

	
	OnHealthPercentUpdateDelegate.Broadcast(

		GetHealth(EStat::Health, EStat::MaxHealth)
		

	);

	if (Stats[EStat::Health] == 0)
	{
		OnZeroHealthDelegate.Broadcast();
	}


}

float UStatsComponent::GetHealth(EStat Current, EStat Max)
{
	
	
	return Stats[Current] / Stats[Max] ;

	
}

