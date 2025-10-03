// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WizPlayeryAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDRUSH_API UWizPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInMelee{ false };
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentVelocity{ 0.0f };

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity();

	

public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocking{ false };
};
