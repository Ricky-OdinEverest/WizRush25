// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeAutoCloseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature,
	UMeleeAutoCloseComponent, OnUpdatedTargetDelegate,
	AActor*, NewTargetActorRef
	
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UMeleeAutoCloseComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* OwnerRef;

	APlayerController* Controller;

	class UCharacterMovementComponent* MovementComp;
	

public:	
	// Sets default values for this component's properties
	UMeleeAutoCloseComponent();
	

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdatedTargetDelegate;

	UPROPERTY(EditAnywhere)
	bool DebugWarpBox = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void StartLockon(double BoxCollisionLength, double BoxCollisionWidth, double BoxCollisionHeight  );
	
	UFUNCTION(BlueprintCallable)
	void StartLockonMotionWarping(double BoxCollisionLength, double BoxCollisionWidth, double BoxCollisionHeight);
	
	UFUNCTION(BlueprintCallable)
	void ToggleLockon(double BoxCollisionLength, double BoxCollisionWidth, double BoxCollisionHeight  );
	
	UFUNCTION(BlueprintCallable)
	void ToggleLockonMotionWarping(double BoxCollisionLength, double BoxCollisionWidth, double BoxCollisionHeight  );


	UFUNCTION(BlueprintCallable)
	void EndLockon();
	
	void SlideTowardsTarget();

	//Warp Target Functions and Variables
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
	
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CurrentTargetActor;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
