// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTT_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDRUSH_API UBTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	
	
	UAnimMontage* CurrentMontage;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* SingleNonHomingMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* TurretMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiNonHomingMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* SingleHomingMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiHomingMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiCombo1Montage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiCombo2Montage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiCombo3Montage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MultiCombo4Montage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* SweepMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AOEMontage;



public:
	UBTT_RangedAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
protected:
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, ACharacter* Character, UBehaviorTreeComponent* OwnerComp);
	
};
