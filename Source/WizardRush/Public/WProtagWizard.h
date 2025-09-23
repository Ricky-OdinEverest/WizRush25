// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/MainCharacterInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "WProtagWizard.generated.h"

class UMotionWarpingComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;



UCLASS()
class WIZARDRUSH_API AWProtagWizard : public ACharacter, public IMainCharacterInterface, public ICombatInterface
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AWProtagWizard();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UMeleeAutoCloseComponent* MeleeAutoCloseComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UMeleeCombatComponent* MeleeCombatComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTraceComponent* TraceComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWizPlayerAnimInstance* PlayerAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWProjectileCombatComponent* ProjectileCombatComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWDashComponent* DashComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* SprintAction;
	
	UMotionWarpingComponent* GetMotionWarpingComponent() const;

	

	
protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarp)
	UMotionWarpingComponent* PlayerMotionWarpingComponent;
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//Movement Functions
	void MoveForward(float  Value);
	void MoveRight(float  Value);
	
	UFUNCTION(BlueprintCallable)
	void SprintStart();
	UFUNCTION(BlueprintCallable)
	void SprintStop();
	UFUNCTION(BlueprintCallable)
	void PrimaryAttackStart();
	UFUNCTION(BlueprintCallable)
	void PrimaryAttackStop();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;

};
