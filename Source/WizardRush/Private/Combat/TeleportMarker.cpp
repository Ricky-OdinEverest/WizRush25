// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TeleportMarker.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATeleportMarker::ATeleportMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

//	MarkerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MarkerMesh"));
//	RootComponent = MarkerMesh;
	//MarkerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATeleportMarker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

