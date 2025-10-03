// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"

USTRUCT(BlueprintType)
struct WIZARDRUSH_API FTraceSockets
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FName Start;

	UPROPERTY(EditAnywhere)
	FName End;

	UPROPERTY(EditAnywhere)
	FName Rotation;
};

UENUM(BlueprintType)
enum class ETraceShape : uint8
{
	Box,
	Capsule
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDRUSH_API UTraceComponent : public UActorComponent
{
	

	GENERATED_BODY()

	USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(EditAnywhere)
	TArray<FTraceSockets> Sockets;

	UPROPERTY(EditAnywhere)
	double BoxCollisionLength{ 30.0 };

	UPROPERTY(EditAnywhere)
	bool bDebugMode{ false };

	TArray<AActor*> TargetsToIgnore;

public:	
	// Sets default values for this component's properties
	UTraceComponent();

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking{ false };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool SocketSweepTrace();
	bool CapsuleSweepTrace();

	UPROPERTY(EditAnywhere, Category="Trace Settings")
	ETraceShape TraceShape = ETraceShape::Capsule;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable)
	void StartTraceWindow();

	UFUNCTION(BlueprintCallable)
	void EndTraceWindow();

	
	UFUNCTION(BlueprintCallable)
	void SetTraceParams(ETraceShape InTraceShape, bool debug);


};
