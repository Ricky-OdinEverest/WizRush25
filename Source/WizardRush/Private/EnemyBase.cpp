// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "DebugHelpers.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::GetHit(const FVector& ImpactPoint, const AActor* Othercomp)
{
	DrawDebugSphere(GetWorld(), ImpactPoint, 50.f, 12, FColor::Orange, false, 5.f);

}

/*
void AEnemyBase::GetHit(const FVector& ImpactPoint)
{
	
	
	
	
	
}
*/

