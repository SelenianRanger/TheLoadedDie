// Copyright © 2022 Sina Kamali


#include "GameFramework/TLDGameUserSettings.h"

bool UTLDGameUserSettings::IsFirstRun() const
{
	return bIsFirstRun;
}

void UTLDGameUserSettings::SetIsFirstRun(bool NewValue)
{
	bIsFirstRun = NewValue;
}

float UTLDGameUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UTLDGameUserSettings::SetMasterVolume(float NewValue)
{
	MasterVolume = NewValue;
}
