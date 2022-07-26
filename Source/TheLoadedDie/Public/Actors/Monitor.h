// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "GameTable.h"
#include "Engine/SceneCapture2D.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

USTRUCT(BlueprintType)
struct FCameraChannel
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<ASceneCapture2D> CCTV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMaterialInstance* MaterialInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChannelChangedDelegate, FCameraChannel, NewCameraChannel);

UCLASS()
class THELOADEDDIE_API AMonitor : public AActor
{
	GENERATED_BODY()

public:

	// table that holds all the stat values for different random events
	UPROPERTY(EditDefaultsOnly, Category="Random Events")
	class UCurveTable* EventStatsTable;
	
	// called when monitor's channel is changed
	UPROPERTY(BlueprintAssignable)
	FOnChannelChangedDelegate OnChannelChanged;

	// Current state of whether the monitor is turned on
	UPROPERTY(BlueprintReadOnly)
	bool bTurnedOn = true;

	// current channel index
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 ChannelIndex = 0;

private:

	float IdleCDMin;
	float IdleCDMax;

	float PowerChance;
	float PowerCDMin;
	float PowerCDMax;

	float RandomChance;
	float RandomCDMin;
	float RandomCDMax;

	FTimerHandle EventTimer;
	
public:

	/**
	 * Toggles powered on state
	 */
	UFUNCTION(BlueprintCallable)
	void TogglePower();

	/**
	 *	Changes monitor's channel to next channel
	 */
	UFUNCTION(BlueprintCallable)
	void GoNextChannel();

	/**
	 * Change monitor's channel to a random channel
	 */
	UFUNCTION(BlueprintCallable)
	void GoRandomChannel();

	/**
	 * Update match result display with new match result
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateMatchResultDisplay(FMatchResult MatchResult);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	
	void ReadStatsFromTable();

	void TriggerEvent();

};
