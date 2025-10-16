// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UMeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackAnimations;

	ACharacter* CharacterRef;

	UPROPERTY(VisibleAnywhere)
	int ComboCounter{ 0 };

public:	
	// Sets default values for this component's properties
	UMeleeCombatComponent();
  
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeleeAttack", meta = (AllowPrivateAccess = "true"))
	bool bIsMeleeAttacking;

	UFUNCTION(BlueprintCallable, Category = "MeleeAttack")
	bool IsMeleeAttacking() const { return bIsMeleeAttacking; }

	UPROPERTY(VisibleAnywhere)
	bool bCanMeleeAttack{ true };

	/*UPROPERTY( EditDefaultsOnly)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY( EditDefaultsOnly)
	USceneComponent* BoxTraceEnd;*/
	
	UPROPERTY( EditDefaultsOnly)
	float BoxX;
	
	UPROPERTY( EditDefaultsOnly)
	float BoxY;
	
	UPROPERTY( EditDefaultsOnly)
	float BoxZ;
	UPROPERTY( EditDefaultsOnly)
	float LengthFromChar = 100;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable)
	void HandleResetCombo();
	
	UFUNCTION(BlueprintCallable)
	void AttackTrace(bool Knockdown) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWindingUp;

	UFUNCTION(BlueprintCallable)
	void WindUpOn() ;

	UFUNCTION(BlueprintCallable)
	void WindUpOff() ;
};
