// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "WActionComponent.generated.h"

class UWAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UWActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor*  Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor*  Instigator, FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void AddAction(TSubclassOf<UWAction> ActionClass);
	
	// Sets default values for this component's properties
	UWActionComponent();

protected:

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<UWAction>> DefaultActions;
	
	UPROPERTY()
	TArray<UWAction*> Actions;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

