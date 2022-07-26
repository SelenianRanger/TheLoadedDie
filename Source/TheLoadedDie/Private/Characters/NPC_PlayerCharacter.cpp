// Copyright © 2022 Sina Kamali 


#include "Characters/NPC_PlayerCharacter.h"

void ANPC_PlayerCharacter::Init(const FNPCSpawnParams &SpawnParams)
{
	bIsCheater = FMath::RandRange(0.f, 1.f) < SpawnParams.CheaterChance;
	bIsSaboteur = bIsCheater && FMath::RandRange(0.f, 1.f) < 0.3;
	bIsStupid = FMath::RandRange(0.f, 1.f) < SpawnParams.StupidChance;
	if(FMath::RandRange(0.f, 1.f) < SpawnParams.RichChance)
	{
		Balance = FMath::RandRange(SpawnParams.RichBalanceRange.MinBalance, SpawnParams.RichBalanceRange.MaxBalance);
	}
	else
	{
		Balance = FMath::RandRange(SpawnParams.NormalBalanceRange.MinBalance, SpawnParams.NormalBalanceRange.MaxBalance);
	}
	InitialBalance = Balance;
	
    Dice = NewObject<UDiceD6>(this, UDiceD6::StaticClass());
	Dice->SetDiceType(bIsCheater ? ELoadedDiceType::EDLT_LoadedHighRoller : ELoadedDiceType::ELDT_NotLoaded);
}

int32 ANPC_PlayerCharacter::GetNextBet() const
{
	float Percentage = 0;
	if(bIsStupid)
	{
		Percentage += FMath::RandRange(0.5f, 1.f);
	}
	else
	{
		Percentage += FMath::RandRange(0.05f, 0.3f);
	}

	if(bIsCheater)
	{
		Percentage += FMath::RandRange(0.1f, 0.3f);
	}

	return FMath::Max(Balance * FMath::Clamp(Percentage, 0.f, 1.f), 100);
	
}

bool ANPC_PlayerCharacter::WantsToLeave() const
{
	// condition if poor
	if(Balance < 100)
	{
		return true;
	}

	float ProfitRatio = (float) Balance / InitialBalance;
	
	// condition if loss is too heavy
	if(ProfitRatio < 0.4f && FMath::RandRange(0.f, 1.f) < 1.f - ProfitRatio)
	{
		return true;
	}

	// condition if profit is high
	if(ProfitRatio > 1.5 && FMath::RandRange(0.f, 1.f) < ProfitRatio / (bIsCheater ? 10.f : 4.f))
	{
		return true;
	}

	// randomly decide to leave
	if(FMath::RandRange(0.f, 1.f) < 0.05f)
	{
		return true;
	}

	return false;
}
