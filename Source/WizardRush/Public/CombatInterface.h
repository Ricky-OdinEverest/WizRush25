// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WIZARDRUSH_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float GetDamage() { return 0.0f; }

	virtual bool CanTakeDamage() { return true; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SendImpact(const FHitResult& InHit) ;

	virtual void GetHitMelee(const FVector& ImpactPoint, const AActor* Othercomp, bool Knockdown) { return; }
	
};
