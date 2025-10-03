// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

class  UNiagaraComponent;


UCLASS()
class WIZARDRUSH_API AWProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack", meta = (ExposeOnSpawn = "true"))
	float ProjectileSpeed = 3000.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
    USphereComponent* SphereComp;
    
   	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
   	UProjectileMovementComponent* MovementComp;
    
   	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
   	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	UNiagaraComponent* NiagaraEffectComp;
   	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
