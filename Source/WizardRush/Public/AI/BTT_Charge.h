// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BehaviorTree/BTTaskNode.h"

#include "BTT_Charge.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDRUSH_API UBTT_Charge : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	double Threshold{ 0.9 };


public:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
