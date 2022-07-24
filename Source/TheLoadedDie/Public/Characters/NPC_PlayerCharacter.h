// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "Actors/GameTable.h"
#include "AI/DiceD6.h"
#include "Characters/NPC_Character.h"
#include "NPC_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THELOADEDDIE_API ANPC_PlayerCharacter : public ANPC_Character
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsStupid = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsCheater = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsSaboteur = false;

	UPROPERTY(BlueprintReadOnly)
	UDiceD6* Dice;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float InitialBalance;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float Balance;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	float AccumulatedProfit = 0;


public:

	/**
	 * called to init values and set player's dice
	 */
	void Init();

	/**
	 * decide on how much to bet on next match
	 */
	UFUNCTION(BlueprintPure)
	int32 GetNextBet() const;

	/**
	 * decide whether NPC wants to stay and keep playing or leave
	 */
	UFUNCTION(BlueprintPure)
	bool WantsToLeave() const;
	
};
