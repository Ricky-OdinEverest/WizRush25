// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Stats/EStat.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthPercentUpdateSignature,
	UStatsComponent, OnHealthPercentUpdateDelegate,
	float, Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnZeroHealthSignature,
	UStatsComponent, OnZeroHealthDelegate
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();
	
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> Stats;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount);

	UFUNCTION(BlueprintPure)
	float GetHealth(EStat Current, EStat Max);

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature OnHealthPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature OnZeroHealthDelegate;


		
};
