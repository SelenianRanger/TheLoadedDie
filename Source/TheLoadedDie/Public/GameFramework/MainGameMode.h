// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "Actors/GameTable.h"
#include "Actors/Monitor.h"
#include "Characters/NPC_PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartDelegate);

UCLASS()
class THELOADEDDIE_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FScoreChangedDelegate OnScoreChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameStartDelegate OnGameStart;

	// Current level of difficulty
	UPROPERTY(BlueprintReadWrite)
	int32 Level;

	// Current distribution parameters of NPCs
	UPROPERTY(BlueprintReadWrite)
	FNPCSpawnParams CurrentSpawnParams;

	// Spawn point for holding idle NPCs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay | Spawn")
	TSoftObjectPtr<class AActor> NPCSpawnPoint;

	// NPC class used to spawn NPCs
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Spawn")
	TSubclassOf<ANPC_PlayerCharacter> NPCClass;

	// Array of all available camera views
	// last element is reserved for off state
	// second to last element is reserved for noise state
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Camera Channels")
	TArray<FCameraChannel> CameraChannels;
	
	// Distribution of each NPC type for each difficulty level
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Difficulty")
	TArray<FNPCSpawnParams> NPCSpawnParams;
	
	// Reference to game tables in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	TArray<TSoftObjectPtr<AGameTable>> GameTables;

	// Reference to monitors in the world
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	TArray<TSoftObjectPtr<AMonitor>> Monitors;

	// Camera actor used for player's view
	UPROPERTY(EditAnywhere, Category="Player | Camera")
	TSoftObjectPtr<ACameraActor> PlayerCamera;


	// game stats
	// -----------------------------------
	// Current Score
	UPROPERTY(BlueprintReadWrite)
	int32 Score = 0;

	// Number of House Wins
	UPROPERTY(BlueprintReadWrite)
	int32 HouseWins = 0;

	// Number of Player Wins
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerWins = 0;

	// Number of Cheaters Caught
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersGot = 0;

	// Number of Cheaters That Got Away
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersMissed = 0;

	// Number of Times Cheaters Won Matches
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersWinCount = 0;

	// Number of Times Cheaters Lost Matches
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersLoseCount = 0;
	
	// Amount Cheaters Made In Profit
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersProfit = 0;

	// Accumulated Percentage Cheaters Made In Profits
	UPROPERTY(BlueprintReadWrite)
	int32 CheatersProfitPercent = 0;
	// -----------------------------------


private:

	UPROPERTY()
	TArray<class ANPC_PlayerCharacter*> IdleNPCPlayers;

public:

	/**
	 * function called to start the game
	 * @param DifficultyLevel: level of difficulty to setup the game
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartGame(int32 DifficultyLevel);

	/**
	 * Asks all idle npcs if they want to take a slot at the request of a table
	 * @return Reference to the NPC that accepted the call. If no one answered returns nullptr.
	 */
	UFUNCTION(BlueprintCallable)
	class ANPC_PlayerCharacter* AskForNPCPlayer();

	/**
	 * Returns an NPC to the Idle queue
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnIdleNPC(ACharacter* Character, bool bIsArrest = false);

	/**
	 * Sends new match result to be displayed on all monitors of the same channel
	 */
	UFUNCTION(BlueprintCallable)
	void BroadcastMatchResultToMonitors(FMatchResult MatchResult, int32 Channel);

	/**
	 * Called to turn off unused scene captures for performance
	 */
	UFUNCTION(BlueprintCallable)
	void OptimizeCameras(FCameraChannel CameraChannel);

protected:
	virtual void BeginPlay() override;

private:

	/**
	 * Spawn a number of NPCs and add them to the idle list
	 */
	void SpawnNPC(int32 Count = 1);

	/**
	 * Calculate new score based on new match result
	 */
	void UpdateScoreStats(FMatchResult MatchResult);
	
};
