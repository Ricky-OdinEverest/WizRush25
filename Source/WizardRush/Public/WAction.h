// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WActionComponent.h"
#include "UObject/NoExportTypes.h"
#include "WAction.generated.h"
class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class WIZARDRUSH_API UWAction : public UObject
{
	GENERATED_BODY()

protected:

	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UWActionComponent* GetOwningComponent() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer BlockedTags;



	bool bIsRunning;

	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor*  Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor*  Instigator);

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;
	
};
