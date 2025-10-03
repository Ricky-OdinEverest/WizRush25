// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BTT_Charge.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UBTT_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ACharacter* CharacterRef{ 
		OwnerComp.GetAIOwner()->GetPawn<ACharacter>() 
	};

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; }

	CharacterRef->PlayAnimMontage(AnimMontage);

	return EBTNodeResult::Succeeded;
}
