// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WDashComponent.generated.h"

class UWProjectileCombatComponent;
class ATeleportMarker;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UWDashComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* CharacterRef;

	APlayerController* Controller;

	class UCharacterMovementComponent* MovementComp;

public:	
	// Sets default values for this component's properties
	UWDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functions For Dash Mechanic   
	UFUNCTION(BlueprintCallable)
	void DashCharge();
	UFUNCTION(BlueprintCallable)
	void DashInitiate();

	UFUNCTION(BlueprintCallable)
	void DashInterrupt();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dashing", meta = (AllowPrivateAccess = "true"))
	bool bIsDashing;

	UFUNCTION(BlueprintCallable, Category = "Dashing")
	bool IsDashing() const { return bIsDashing; }

	// Teleport Marker Management
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dashing")
	TSubclassOf<ATeleportMarker> TeleportMarkerClass;

	
    
private:

	UWProjectileCombatComponent* ProjectileCombatComponent; // Reference to ProjectileCombatComponent
	
	float DashChargeTime;  // To track how long the dash input is held
	
	const float MaxDashDistance = 2000.0f;  // Maximum distance to dash

	// Teleport Marker Instance
	UPROPERTY()
	ATeleportMarker* TeleportMarkerInstance;
};
