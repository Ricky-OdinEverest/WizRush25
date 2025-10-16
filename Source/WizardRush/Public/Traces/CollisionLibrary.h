// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDRUSH_API UCollisionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Collision", Meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Cone Trace By Channel", AdvancedDisplay = "TraceColor, TraceHitColor, DrawTime", Keywords = "sweep"))
	static bool ConeTraceMulti(const UObject* WorldContextObject, const FVector Start, const FRotator Direction,
		float ConeHeight, float ConeHalfAngle, float ConeRadius, ETraceTypeQuery TraceChannel, bool bTraceComplex,
		const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits,
		FLinearColor TraceColor = FLinearColor(0.0f, 1.0f, 1.0f),
		FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);


	UFUNCTION(BlueprintCallable, Category = "Collision", Meta = (WorldContext = "WorldContextObject"))
	static void CheckRadialSector(const UObject* WorldContextObject, FVector Origin, FVector Forward, float Radius, float ArcAngleDegrees, ECollisionChannel CollisionChannel,  AActor* IgnoredActor = nullptr);


	UFUNCTION(BlueprintCallable, Category = "Collision", Meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Radial Sector Trace Multi By Channel", AdvancedDisplay = "TraceColor, TraceHitColor, DrawTime", Keywords = "sweep"))
	static bool RadialSectorTraceMulti(const UObject* WorldContextObject, const FVector Start, const FRotator Direction,
		 float ConeHalfAngle,  float InnerCircleRadius, float OuterCircleRadius, ETraceTypeQuery TraceChannel, bool bTraceComplex,
		const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits,
		FLinearColor TraceColor = FLinearColor(0.0f, 1.0f, 1.0f),
		FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);


	
	static void DrawDebugArcSegment(
		UWorld* World,
		FVector Origin,
		FVector Forward,
		float Radius,
		float AngleDegrees,
		int32 SegmentCount,
		FColor Color,
		bool bPersistentLines,
		float LifeTime,
		uint8 DepthPriority = 0,
		float Thickness = 1.0f
	);
};
