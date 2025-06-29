// Fill out your copyright notice in the Description page of Project Settings.


#include "WActionComponent.h"

#include "WAction.h"

// Sets default values for this component's properties
UWActionComponent::UWActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UWActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for(TSubclassOf<UWAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
	
}

void UWActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Red, DebugMsg);
	
}


void UWActionComponent::AddAction(TSubclassOf<UWAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}
	UWAction* NewAction = NewObject<UWAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UWActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UWAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, FailedMsg);
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UWActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UWAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}




