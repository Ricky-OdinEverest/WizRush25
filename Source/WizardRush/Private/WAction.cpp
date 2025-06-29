// Fill out your copyright notice in the Description page of Project Settings.


#include "WAction.h"


void UWAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	UWActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UWAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Completed: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);
	
	UWActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	bIsRunning = false;
}




UWorld* UWAction::GetWorld() const
{
	//Outer is Set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
UWActionComponent* UWAction::GetOwningComponent() const
{
	return Cast<UWActionComponent>(GetOuter());
}

bool UWAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	UWActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool UWAction::IsRunning() const
{
	return bIsRunning;
}
