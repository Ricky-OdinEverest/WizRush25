// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriangleExpander.generated.h"

UCLASS()
class WIZARDRUSH_API ATriangleExpander : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriangleExpander();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void  BuildTriangularGrid(
	int32 Rows, int32 Cols, float a,
	TArray<FVector>& Vertices, TArray<int32>& Indices,
	TArray<FVector2D>& UVs);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProcMesh;

};
