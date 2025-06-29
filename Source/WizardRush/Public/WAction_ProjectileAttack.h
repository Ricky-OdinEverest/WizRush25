// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "WAction.h"
#include "WAction_ProjectileAttack.generated.h"

class UWDashComponent;
UCLASS()
class WIZARDRUSH_API UWAction_ProjectileAttack : public UWAction
{
	GENERATED_BODY()

protected:
	ACharacter* CharacterRef;

	APlayerController* Controller;

	class UCharacterMovementComponent* MovementComp;

	class UWDashComponent* DashComponent;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	
	UPROPERTY(BlueprintReadOnly)
	class UWizPlayerAnimInstance* PlayerAnim;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* CastingEffects;




public:	
	// Sets default values for this component's properties
	UWAction_ProjectileAttack();

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StartAiming(ACharacter* InstigatorCharacter);  // Function to start aiming
	
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void StopAiming(ACharacter* InstigatorCharacter);   // Function to stop aiming
	
	// Blueprint getter function to check if aiming
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	bool IsAiming() const { return bIsAiming; }

	// Flag to indicate if the character is currently aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
	
	void RotateCharacterToMouseCursor(ACharacter* InstigatorCharacter);
	

	// Blueprint getter function to check if Shooting
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	bool IsShooting() const { return bIsShooting; }


	// Flag to indicate if the character is currently Shooting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	bool bIsShooting;
	
	// Cached direction from character to mouse position
	FVector CachedMouseDirection;

	FTimerHandle TimerHandle_AttackDelay;
    
	// Function to handle starting and stopping the repeating attack
	void StartRepeatingAttack(ACharacter* InstigatorCharacter);
	void StopRepeatingAttack(ACharacter* InstigatorCharacter);

	

	UFUNCTION()
	void PrimaryAttack_TimeElapsed(ACharacter* InstigatorCharacter);

	void PrimaryAttack(ACharacter* InstigatorCharacter);

	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
