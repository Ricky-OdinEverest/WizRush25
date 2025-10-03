// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_RangedAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WizardTypes.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ 
		OwnerComp.GetAIOwner()->GetPawn<ACharacter>() 
	};

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; }

	CharacterRef->PlayAnimMontage(AnimMontage);




	return EBTNodeResult::Succeeded;
}

/*
double RandomValue{ UKismetMathLibrary::RandomFloat() };

if (RandomValue > Threshold)
{
	Threshold = 0.9;
	
	OwnerComp.GetBlackboardComponent()->
	SetValueAsEnum(TEXT("CurrentState"),
		EEnemyState::Charge);
}
else
{
	Threshold -= 0.1;
}
*/
