// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WProjectileCombatComponent.generated.h"

class UWDashComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UWProjectileCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* CharacterRef;

	APlayerController* Controller;

	class UCharacterMovementComponent* MovementComp;

	UWDashComponent* DashComponent;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	//UPROPERTY(EditAnywhere, Category = "Attack")
	//UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	// no need to set variable in editor 
	UPROPERTY(BlueprintReadOnly)
	class UWizPlayerAnimInstance* PlayerAnim;



public:	
	// Sets default values for this component's properties
	UWProjectileCombatComponent();
//not sure what the best access mod is for components
//protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Aiming control functions
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StartAiming();  // Function to start aiming
	
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StopAiming();   // Function to stop aiming
	
	// Blueprint getter function to check if aiming
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	bool IsAiming() const { return bIsAiming; }

	// Flag to indicate if the character is currently aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aiming", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;


	// Calculate the new yaw rotation based on mouse position
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	void RotateCharacterToMouseCursor();
	

	// Blueprint getter function to check if Shooting
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	bool IsShooting() const { return bIsShooting; }


	// Flag to indicate if the character is currently Shooting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	bool bIsShooting;
	

	

	
	// Cached direction from character to mouse position
	FVector CachedMouseDirection; 
    
	// Function to handle starting and stopping the repeating attack
	void StartRepeatingAttack();
	void StopRepeatingAttack();

	void PrimaryAttack_TimeElapsed();
	void PrimaryAttack();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
