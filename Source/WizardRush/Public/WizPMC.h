// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WizPMC.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDRUSH_API UWizPMC : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
protected:
	
	virtual UProjectileMovementComponent::EHandleBlockingHitResult HandleBlockingHit(
		const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;
	
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJuggernaught = false;
};
