// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EStat
{
	None UMETA(DisplayName = "None Selected"),
	Health UMETA(DisplayName = "Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Magic UMETA(DisplayName = "Magic"),
	MaxMagic UMETA(DisplayName = "Max Magic")
};

UENUM(BlueprintType)
enum EHitType
{
	Default UMETA(DisplayName = "Default"),
	Heavy UMETA(DisplayName = "Heavy"),
	Light UMETA(DisplayName = "Light"),
	Knockdown UMETA(DisplayName = "Knockdown"),
	
};
