// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DiceD6.generated.h"

UENUM(BlueprintType)
enum class ELoadedDiceType : uint8
{
	ELDT_NotLoaded			UMETA(DisplayName="Not Loaded"),
	EDLT_LoadedHighRoller	UMETA(DisplayName="Loaded High Roller"),
	EDLT_LoadedLowRoller	UMETA(DisplayName="Loaded Low Roller")
};

UCLASS()
class THELOADEDDIE_API UDiceD6 : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	float RollWeights[6];

public:

	UFUNCTION(BlueprintCallable)
    void SetDiceType(ELoadedDiceType LoadedDice);

	UFUNCTION(BlueprintPure)
	int32 Roll() const;
	
};
