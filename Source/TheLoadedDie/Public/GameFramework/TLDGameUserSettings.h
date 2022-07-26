// Copyright © 2022 Sina Kamali

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TLDGameUserSettings.generated.h"

UCLASS(BlueprintType)
class THELOADEDDIE_API UTLDGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

protected:

	UPROPERTY(Config)
	bool bIsFirstRun;

	UPROPERTY(Config)
	float MasterVolume;

public:

	UFUNCTION(BlueprintCallable, Category=Settings)
	bool IsFirstRun() const;

	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetIsFirstRun(bool NewValue);

	UFUNCTION(BlueprintCallable, Category=Settings)
	float GetMasterVolume() const;

	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetMasterVolume(float NewValue);
	
};
