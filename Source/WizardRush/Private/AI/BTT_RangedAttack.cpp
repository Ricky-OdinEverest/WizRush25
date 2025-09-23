// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_RangedAttack.h"

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Success!"));

	return EBTNodeResult::Succeeded;
}

