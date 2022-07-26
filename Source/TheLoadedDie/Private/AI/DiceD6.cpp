// Copyright © 2022 Sina Kamali 


#include "AI/DiceD6.h"

void UDiceD6::SetDiceType(ELoadedDiceType LoadedDice)
{
	switch(LoadedDice)
	{
	case ELoadedDiceType::ELDT_NotLoaded:
		for (int i = 0; i < 6; ++i)
		{
			RollWeights[i] = (1.f);
		}
		break;

	case ELoadedDiceType::EDLT_LoadedHighRoller:
		for (int i = 0; i < 4; ++i)
		{
			RollWeights[i] = (FMath::RandRange(0.5f, 1.f));
		}
		for (int i = 4; i < 6; ++i)
		{
			RollWeights[i] = (FMath::RandRange(3.f, 4.f));
		}
		break;

	case ELoadedDiceType::EDLT_LoadedLowRoller:
		for (int i = 0; i < 3; ++i)
		{
			RollWeights[i] = (FMath::RandRange(3.f, 4.f));
		}
		for (int i = 4; i < 6; ++i)
		{
			RollWeights[i] = (FMath::RandRange(0.5f, 1.f));
		}
		break;

	default:
		break;
	}
}

int32 UDiceD6::Roll() const
{
	float TotalWeight = 0;
	for (int i = 0; i < 6; ++i)
	{
		TotalWeight += RollWeights[i];
	}

	float Roll = FMath::RandRange(0.f, TotalWeight);
	float Count = 0;
	for (int i = 0; i < 6; ++i)
	{
		Count += RollWeights[i];
		if(Count >= Roll)
		{
			return i + 1;
		}
	}

	return 5;
}
