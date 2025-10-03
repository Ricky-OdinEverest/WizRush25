// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyState
{
	Idle UMETA(DisplayName="Idle"),
	Range UMETA(DisplayName="Range"),
	Charge UMETA(DisplayName="Charge"),
	Melee UMETA(DisplayName="Melee"),
	Aggressive UMETA(DisplayName="Aggressive"),
	Reposition UMETA(DisplayName="Reposition"),
	GameOver UMETA(DisplayName="Game Over"),
};


enum ERangeType
{
	SingleNonHoming UMETA(DisplayName="SingleNonHoming"),
	MultiNonHoming UMETA(DisplayName="MultiNonHoming"),
	SingleHoming UMETA(DisplayName="SingleHoming"),
	MultiHoming UMETA(DisplayName="MultiHoming"),
	AOE UMETA(DisplayName="AOE")
};
