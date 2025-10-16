// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_RangedAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WizardTypes.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_RangedAttack::UBTT_RangedAttack()
{
	bNotifyTick = false; // No need to tick every frame
	CurrentMontage = nullptr;
}

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACharacter* CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
    if (!IsValid(CharacterRef))
    {
        return EBTNodeResult::Failed;
    }

    // Retrieve the RangeType from blackboard
    uint8 RangeTypeValue = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(TEXT("RangeType"));
    ERangeType RangeType = static_cast<ERangeType>(RangeTypeValue);

    // Pick the correct montage
    UAnimMontage* MontageToPlay = nullptr;
    switch (RangeType)
    {
        case SingleNonHoming: MontageToPlay = SingleNonHomingMontage; break;
        case Turret: MontageToPlay = TurretMontage; break;
        case MultiNonHoming: MontageToPlay = MultiNonHomingMontage; break;
        case SingleHoming: MontageToPlay = SingleHomingMontage; break;
        case MultiHoming: MontageToPlay = MultiHomingMontage; break;
        case MultiCombo1: MontageToPlay = MultiCombo1Montage; break;
        case MultiCombo2: MontageToPlay = MultiCombo2Montage; break;
        case MultiCombo3: MontageToPlay = MultiCombo3Montage; break;
        case MultiCombo4: MontageToPlay = MultiCombo4Montage; break;
        case Sweep: MontageToPlay = SweepMontage; break;
        case AOE: MontageToPlay = AOEMontage; break;
        default: break;
    }

    if (!MontageToPlay)
    {
        UE_LOG(LogTemp, Warning, TEXT("No montage found for range type"));
        return EBTNodeResult::Failed;
    }

    // Play the montage and bind to the end event
    CurrentMontage = MontageToPlay;

    if (UAnimInstance* AnimInstance = CharacterRef->GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(MontageToPlay);

        // Create a dynamic delegate for when montage ends
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &UBTT_RangedAttack::OnMontageEnded, CharacterRef, &OwnerComp);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);

        return EBTNodeResult::InProgress; // wait until animation ends
    }

    return EBTNodeResult::Failed;
}

void UBTT_RangedAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, ACharacter* Character,
	UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp || !Character)
		return;

	// Only finish if this is the montage we started
	if (Montage == CurrentMontage)
	{
		if (bInterrupted)
		{
			OwnerComp->OnTaskFinished(this, EBTNodeResult::Failed);
		}
		else
		{
			OwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
	}
}
