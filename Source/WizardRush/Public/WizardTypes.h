// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyState
{
	Idle UMETA(DisplayName="Idle"),
	Range UMETA(DisplayName="Range"),
	Block UMETA(DisplayName="Block"),
	Aggressive UMETA(DisplayName="Aggressive"),
	Reposition UMETA(DisplayName="Reposition"),
	TakingDamage UMETA(DisplayName="TakingDamage"),
	GameOver UMETA(DisplayName="Game Over"),
};

UENUM(BlueprintType)
enum EHandleDamage
{
	RangeDamage UMETA(DisplayName="RangeDamage"),
	MeleeDamage UMETA(DisplayName="MeleeDamage")
	
};


UENUM(BlueprintType)
enum EMeleeType
{
	Charge UMETA(DisplayName="Charge"),
	Melee UMETA(DisplayName="Melee"),
	LeapToAttack UMETA(DisplayName="LeapToAttack")
};


UENUM(BlueprintType)
enum ERangeType
{
	SingleNonHoming UMETA(DisplayName="SingleNonHoming"),
	Turret UMETA(DisplayName="Turret"),
	MultiNonHoming UMETA(DisplayName="MultiNonHoming"),
	SingleHoming UMETA(DisplayName="SingleHoming"),
	MultiHoming UMETA(DisplayName="MultiHoming"),
	MultiCombo1 UMETA(DisplayName="MultiCombo1"),
	MultiCombo2 UMETA(DisplayName="MultiCombo2"),
	MultiCombo3 UMETA(DisplayName="MultiCombo3"),
	MultiCombo4 UMETA(DisplayName="MultiCombo4"),
	Sweep UMETA(DisplayName="Sweep"),
	AOE UMETA(DisplayName="AOE")
};

UENUM(BlueprintType)
enum ERepositionType
{
	LeapAway UMETA(DisplayName="LeapAway"),
	Teleport UMETA(DisplayName="Teleport"),
};
