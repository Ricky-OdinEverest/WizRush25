// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "WizardTypes.h"
#include "BossCharacter.generated.h"

UCLASS()
class WIZARDRUSH_API ABossCharacter : public ACharacter, public IEnemyInterface, public ICombatInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReactAnimMontage;


public:
	// Sets default values for this character's properties
	ABossCharacter();


	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnim;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere)
	UAnimMontage* KnockdownAnim;

	UPROPERTY(EditAnywhere)
	UAnimMontage* GetUpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Invincible;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKnockedDown;
	

	UPROPERTY(EditAnywhere, Category = "VFX")
	class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* HitSound;
	
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APawn* CombatTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;
 
	UPROPERTY()
	TObjectPtr<class AAIController> EnemyAIController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	virtual void GetHit(const FVector& ImpactPoint, const AActor* Othercomp) override;
	
	virtual void GetHitMelee(const FVector& ImpactPoint, const AActor* Othercomp, bool Knockdown) override;

	virtual float GetDamage() override;

	UFUNCTION()
	void HandlePlayerDeath();

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	UFUNCTION()
	void FinishDeathAnim();

	//UFUNCTION(BlueprintCallable)
	void PlayHurtAnim(bool Knockdown);

	void GetUp(UAnimMontage* Montage, bool bInterrupted);
	
	void RecoveredFromKnockdown(UAnimMontage* Montage, bool bInterrupted);
};



