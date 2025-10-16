// Fill out your copyright notice in the Description page of Project Settings.
//ref : https://sreitich.github.io/cone-trace/

#include "Traces/CollisionLibrary.h"

#include "KismetTraceUtils.h"
#include "Engine/OverlapResult.h"

bool UCollisionLibrary::ConeTraceMulti(const UObject* WorldContextObject, const FVector Start, const FRotator Direction,
                                       float ConeHeight, float ConeHalfAngle, float ConeRadius, ETraceTypeQuery TraceChannel, bool bTraceComplex,
                                       const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits,
                                       FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	OutHits.Reset();
    
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
    
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}
   
	TArray<FHitResult> TempHitResults;
	const FVector End = Start + (Direction.Vector() * ConeHeight);
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);
	// r = h * tan(theta / 2)
	const double ConeBaseRadius = ConeHeight * tan(ConeHalfAngleRad);
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);

	// Perform a sweep encompassing an imaginary cone.
	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);

	// Filter for hits that would be inside the cone.
	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		// theta = arccos((A • B) / (|A|*|B|)). |A|*|B| = 1 because A and B are unit vectors.
		const double DeltaAngle = FMath::Acos(Dot);

		// Hit is outside the angle of the cone.
		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}

		const double Distance = (HitResult.ImpactPoint - Start).Length();
		// Hypotenuse = adjacent / cos(theta)
		const double LengthAtAngle = ConeHeight / cos(DeltaAngle);

		// Hit is beyond the cone. This can happen because we sweep with spheres, which results in a cap at the end of the sweep.
		if (Distance > LengthAtAngle)
		{
			continue;
		}

		OutHits.Add(HitResult);
	}
#if ENABLE_DRAW_DEBUG
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// Cone trace.
		const double ConeSlantHeight = FMath::Sqrt((ConeBaseRadius * ConeBaseRadius) + (ConeHeight * ConeHeight)); // s = sqrt(r^2 + h^2)
		DrawDebugCone(World, Start, Direction.Vector(), ConeSlantHeight, ConeHalfAngleRad, ConeHalfAngleRad, 32, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
        
		// Uncomment to see the trace we're actually performing.
		// DrawDebugSweptSphere(World, Start, End, ConeBaseRadius, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
        
		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, true, Hit, TraceHitColor, TraceHitColor, DrawTime);
		}
        
		// Uncomment to see hits from the sphere sweep that were filtered out.
		// for (const FHitResult& Hit : TempHitResults)
		// {
		//     if (!OutHits.ContainsByPredicate([Hit](const FHitResult& Other)
		//     {
		//         return (Hit.GetActor() == Other.GetActor()) &&
		//                (Hit.ImpactPoint == Other.ImpactPoint) &&
		//                (Hit.ImpactNormal == Other.ImpactNormal);
		//     }))
		//     {
		//         DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, false, Hit, FColor::Red, FColor::Red, DrawTime);
		//     }
		// }
	}
#endif // ENABLE_DRAW_DEBUG

	return (OutHits.Num() > 0);
}

void UCollisionLibrary::CheckRadialSector(
	const UObject* WorldContextObject,   // Context object used to get a reference to the world
	FVector Origin,                      // Center of the radial sector (e.g., boss location)
	FVector Forward,                     // Direction the sector is facing (e.g., boss forward vector)
	float Radius,                        // Current size of the sector (expands over time)
	float ArcAngleDegrees,
	ECollisionChannel CollisionChannel, // Total angle of the sector in degrees (e.g., 90° cone)
	AActor* IgnoredActor /*= nullptr*/   // Optional actor to ignore during the check (usually self)
)
{
	// Ensure a valid world context was provided
	if (!WorldContextObject) return;

	// Get a reference to the game world from the context object
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return;

	// Convert the total arc angle to radians and divide by 2 to get the half-angle
	float HalfAngleRad = FMath::DegreesToRadians(ArcAngleDegrees * 0.5f);

	// --- Debug Visualization ---

	{
		FColor ArcColor = FColor::Purple;

		// Draw a visual cone in the world to represent the sector shape
		DrawDebugCone(
			World,
			Origin,              // Cone start point
			Forward,             // Direction the cone faces
			Radius,              // Length of the cone
			HalfAngleRad,        // Half-angle in one direction (left)
			HalfAngleRad,        // Half-angle in the other direction (right)
			32,                  // Number of lines to draw the cone shape
			ArcColor,            // Cone color
			false,               // Not persistent
			3.1f,                // Display time
			0,                   // Depth priority
			1.f                  // Line thickness
		);

	}



	// --- Collision Overlap ---

	// Holds the list of hit actors within the sphere
	TArray<FOverlapResult> Overlaps;

	// Create a spherical collision shape with the current radius
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	// Setup collision parameters and ignore the specified actor (usually self)
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(IgnoredActor);

	// Check for all actors overlapping the sphere using the Pawn collision channel
	World->OverlapMultiByChannel(
		Overlaps,
		Origin,            // Center of the overlap sphere
		FQuat::Identity,   // No rotation needed for sphere
		CollisionChannel,          // Collision channel to trace against
		Sphere,            // Shape of the trace
		Params             // Query params (including ignored actors)
	);

	// --- Filter Hits by Angle ---

	for (auto& Result : Overlaps)
	{
		AActor* Actor = Result.GetActor();

		// Skip if the actor is null or is the one we’re ignoring
		if (!Actor || Actor == IgnoredActor) continue;

		// Calculate vector from origin to target
		FVector ToTarget = (Actor->GetActorLocation() - Origin);

		// Normalize the direction vector
		FVector ToTargetNorm = ToTarget.GetSafeNormal();

		// Get the dot product between forward direction and direction to target
		float Dot = FVector::DotProduct(Forward, ToTargetNorm);

		// Use arccosine to convert dot product to angle in radians
		float AngleToTarget = FMath::Acos(Dot);

		// If target is within the sector's angle
		if (AngleToTarget <= HalfAngleRad)
		{
			// ✅ Target is inside the radial sector
			UE_LOG(LogTemp, Log, TEXT("[RadialSector] Hit: %s"), *Actor->GetName());

			// Draw a debug line from origin to the hit actor
			DrawDebugLine(
				World,
				Origin,
				Actor->GetActorLocation(),
				FColor::Green,
				true,
				5.f,
				0,
				2.0f
			);

			// Draw a debug sphere at the actor’s location
			DrawDebugSphere(
				World,
				Actor->GetActorLocation(),
				30.f,          // Radius of sphere
				12,            // Segments
				FColor::Green,
				true,
				1.f           // Display time
			);
		}
	}
}

bool UCollisionLibrary::RadialSectorTraceMulti(const UObject* WorldContextObject, const FVector Start,
	const FRotator Direction, float ConeHalfAngle, float InnerCircleRadius, float OuterCircleRadius, ETraceTypeQuery TraceChannel,
	bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType,
	TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
		OutHits.Reset();
    
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
    
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}
   
	TArray<FHitResult> TempHitResults;
	
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);

	float ArcRadius = OuterCircleRadius; 
	// Use: h = r / tan(θ)
	float AdjustedConeHeight = ArcRadius / FMath::Tan(ConeHalfAngleRad);

	const FVector End = Start + (Direction.Vector() * AdjustedConeHeight);
	const double ConeBaseRadius = ArcRadius; // Outer radius of the arc

	
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);

	

	// Perform a sweep encompassing an imaginary cone. // (changed to in place )
	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);

	// Filter for hits that would be inside the cone.
	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		// theta = arccos((A • B) / (|A|*|B|)). |A|*|B| = 1 because A and B are unit vectors.
		const double DeltaAngle = FMath::Acos(Dot);

		// Hit is outside the angle of the cone.
		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}

		//const double Distance = (HitResult.ImpactPoint - Start).Length();
		// Hypotenuse = adjacent / cos(theta)
		const double LengthAtAngle = AdjustedConeHeight / cos(DeltaAngle);
		//Allow the round shape at the end for the arc
		
		// Hit is inside the safe area of the circle near the start of the cone 

		const double DistanceToHit = FVector::Dist(Start, HitResult.ImpactPoint);

		if (DistanceToHit < InnerCircleRadius)
		{
			// Try to recover valid hits by tracing through the "donut ring"
			const FVector DirectionToHit = (HitResult.ImpactPoint - Start).GetSafeNormal();

			FVector RingStart = Start + DirectionToHit * InnerCircleRadius;
			//catch overlapping player
			const float RecoveryTraceBuffer = 50.0f;
			FVector RingEnd = Start + DirectionToHit * (OuterCircleRadius + RecoveryTraceBuffer);

			FHitResult SecondaryHit;
			FCollisionQueryParams LineParams = Params; // reuse original params
			

			bool bHit = World->LineTraceSingleByChannel(SecondaryHit, RingStart, RingEnd, CollisionChannel, LineParams);

			

			if (bHit && SecondaryHit.GetActor() == HitResult.GetActor())
			{
				float RingHitDistance = FVector::Dist(RingStart, SecondaryHit.ImpactPoint);

				if (RingHitDistance <= (OuterCircleRadius - InnerCircleRadius))
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
							FString::Printf(TEXT("Recovered hit on %s via secondary trace!"), *HitResult.GetActor()->GetName()));
					}

					UE_LOG(LogTemp, Warning, TEXT("Recovered hit on %s (RingHitDistance: %.2f)"),
						*HitResult.GetActor()->GetName(), RingHitDistance);

					OutHits.Add(HitResult); // Use original hit or SecondaryHit as needed
					continue; // Skip 'continue' below
				}
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Too close: Distance = %.2f"), DistanceToHit));
			}

			UE_LOG(LogTemp, Warning, TEXT("Skipped: DistanceToHit < InnerCircleRadius (%.2f < %.2f)"), DistanceToHit, InnerCircleRadius);
			continue;
		}
///////////////

		if (DistanceToHit > OuterCircleRadius)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Too far: Distance = %.2f"), DistanceToHit));
			}
			UE_LOG(LogTemp, Warning, TEXT("Skipped: DistanceToHit > OuterCircleRadius (%.2f > %.2f)"), DistanceToHit, OuterCircleRadius);
			continue;
		}


		// Hit is beyond the cone. This can happen because we sweep with spheres, which results in a cap at the end of the sweep.
		/*if (DistanceToHit > LengthAtAngle)
		{
			continue;
		}*/
		

		OutHits.Add(HitResult);
	}
#if ENABLE_DRAW_DEBUG
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		/*
		// Cone trace.
		//const double ConeSlantHeight = FMath::Sqrt((ConeBaseRadius * ConeBaseRadius) + (AdjustedConeHeight * AdjustedConeHeight)); // s = sqrt(r^2 + h^2)
		DrawDebugCone(World,
			Start,
			Direction.Vector(),
			AdjustedConeHeight,
			ConeHalfAngleRad,
			ConeHalfAngleRad,
			32,
			TraceColor.ToFColor(true),
			(DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);

		DrawDebugArcSegment(
			World,
			Start,
			Direction.Vector(),
			InnerCircleRadius,
			ConeHalfAngle * 2.f, // full cone angle
			32,
			FColor::Red,
			DrawDebugType == EDrawDebugTrace::Persistent,
			DrawTime
		);

		DrawDebugArcSegment(
			World,
			Start,
			Direction.Vector(),
			ConeBaseRadius,
			ConeHalfAngle * 2.f,
			32,
			FColor::Green,
			DrawDebugType == EDrawDebugTrace::Persistent,
			DrawTime
		);
			        
		// Uncomment to see the trace we're actually performing.
		 DrawDebugSweptSphere(World, Start, End, ConeBaseRadius, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
		 */
        
		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, true, Hit, TraceHitColor, TraceHitColor, DrawTime);
		}
        
		// Uncomment to see hits from the sphere sweep that were filtered out.
		for (const FHitResult& Hit : TempHitResults)
		 {
		     if (!OutHits.ContainsByPredicate([Hit](const FHitResult& Other)
		     {
		         return (Hit.GetActor() == Other.GetActor()) &&
		                (Hit.ImpactPoint == Other.ImpactPoint) &&
		                (Hit.ImpactNormal == Other.ImpactNormal);
		     }))
		     {
		         DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, false, Hit, FColor::Red, FColor::Red, DrawTime);
		     }
		 }
	}
#endif // ENABLE_DRAW_DEBUG

	return (OutHits.Num() > 0);
}

void UCollisionLibrary::DrawDebugArcSegment(UWorld* World, FVector Origin, FVector Forward, float Radius,
	float AngleDegrees, int32 SegmentCount, FColor Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority,
	float Thickness)
{
	if (!World || SegmentCount <= 1) return;

	const float HalfAngleRad = FMath::DegreesToRadians(AngleDegrees * 0.5f);
	const FVector Right = FVector::CrossProduct(Forward, FVector::UpVector).GetSafeNormal();
	const FVector Up = FVector::CrossProduct(Right, Forward).GetSafeNormal();

	for (int32 i = 0; i < SegmentCount - 1; ++i)
	{
		float T0 = (float)i / (SegmentCount - 1);
		float T1 = (float)(i + 1) / (SegmentCount - 1);

		float Angle0 = FMath::Lerp(-HalfAngleRad, HalfAngleRad, T0);
		float Angle1 = FMath::Lerp(-HalfAngleRad, HalfAngleRad, T1);

		FVector Point0 = Origin + (Forward.RotateAngleAxis(FMath::RadiansToDegrees(Angle0), Up) * Radius);
		FVector Point1 = Origin + (Forward.RotateAngleAxis(FMath::RadiansToDegrees(Angle1), Up) * Radius);

		DrawDebugLine(World, Point0, Point1, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
}
