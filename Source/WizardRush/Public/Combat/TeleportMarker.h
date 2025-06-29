// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportMarker.generated.h"

class USkeletalMesh;
UCLASS()
class WIZARDRUSH_API ATeleportMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportMarker();
	
	//UPROPERTY(VisibleAnywhere , BlueprintReadWrite, Category = "Teleport Marker")
	//USkeletalMeshComponent* MarkerMesh;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
