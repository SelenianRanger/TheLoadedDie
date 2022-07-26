// Copyright © 2022 Sina Kamali 

#include "GameFramework/MainGameMode.h"

#include "Camera/CameraActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

ANPC_PlayerCharacter* AMainGameMode::AskForNPCPlayer()
{
	for (ANPC_PlayerCharacter* NPC : IdleNPCPlayers)
	{
		if(FMath::RandRange(0.f, 1.f) < CurrentSpawnParams.AnswerRate)
		{
			IdleNPCPlayers.Remove(NPC);
			return NPC;
		}
	}
	
	return nullptr;
}

void AMainGameMode::ReturnIdleNPC_Implementation(ACharacter* Character, bool bIsArrest)
{
	ANPC_PlayerCharacter* NPC = Cast<ANPC_PlayerCharacter>(Character);
	if(!NPC)
	{
		return;
	}

	if(bIsArrest)
	{
		CheatersGot += NPC->bIsCheater ? 1 : 0;
		NPC->Destroy();
		SpawnNPC();
		return;
	}
	
	if(NPC->WantsToLeave())
	{
		CheatersMissed += NPC->bIsCheater ? 1 : 0;
		CheatersProfit += NPC->bIsCheater ? NPC->Balance - NPC->InitialBalance : 0;
		CheatersProfitPercent += NPC->bIsCheater ? NPC->AccumulatedProfit : 0;
		NPC->Destroy();
		SpawnNPC();
		return;
	}

	NPC->AccumulatedProfit += FMath::Floor((float) NPC->Balance / NPC->InitialBalance * 100);

	// if player wants to stay in casino, update the new initial balance for profit calculation
	NPC->InitialBalance = NPC->Balance;
	
	IdleNPCPlayers.Add(NPC);
}

void AMainGameMode::BroadcastMatchResultToMonitors(FMatchResult MatchResult ,int32 Channel)
{
	for (TSoftObjectPtr<AMonitor> Monitor : Monitors)
	{
		if(Monitor.Get()->ChannelIndex == Channel)
		{
			Monitor.Get()->UpdateMatchResultDisplay(MatchResult);
		}
	}
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// setup camera
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(PlayerController)
	{
		if(PlayerCamera.IsValid())
		{
			PlayerController->SetViewTarget(PlayerCamera.Get());
		}
	}

	for(int i = 0; i < GameTables.Num(); ++i)
	{
		GameTables[i]->OnNewMatchEndedInternal.AddUObject(this, &AMainGameMode::BroadcastMatchResultToMonitors, i);
		GameTables[i]->OnNewMatchEndedInternal.AddUObject(this, &AMainGameMode::UpdateScoreStats);
	}

	for(TSoftObjectPtr<AMonitor> Monitor : Monitors)
	{
		if(Monitor.IsValid())
		{
			Monitor.Get()->OnChannelChanged.AddDynamic(this, &AMainGameMode::OptimizeCameras);
		}
	}
	
}

void AMainGameMode::UpdateScoreStats(FMatchResult MatchResult)
{
	Score += MatchResult.Bet * (MatchResult.PlayerWon ? -1 : 1);
	HouseWins += MatchResult.PlayerWon ? 0 : 1;
	PlayerWins += MatchResult.PlayerWon ? 1 : 0;
	
	OnScoreChanged.Broadcast(Score);
}

void AMainGameMode::OptimizeCameras(FCameraChannel CameraChannel)
{
	bool* CaptureOn = new bool[CameraChannels.Num() - 2];
	for (int i = 0; i < CameraChannels.Num() - 2; ++i)
	{
		CaptureOn[i] = false;
	}
	
	for (int i = 0; i < Monitors.Num(); ++i)
	{
		CaptureOn[Monitors[i]->ChannelIndex] = Monitors[i]->bTurnedOn || CaptureOn[Monitors[i]->ChannelIndex];
	}

	for(int i = 0; i < CameraChannels.Num() - 2; ++i)
	{
		if(CameraChannels[i].CCTV.IsValid())
		{
			CameraChannels[i].CCTV.Get()->GetCaptureComponent2D()->SetVisibility(CaptureOn[i]);
		}
	}
}

void AMainGameMode::SpawnNPC(int32 Count)
{
	if(!NPCSpawnPoint.IsValid())
	{
		return;
	}
	
	for (int i = 0; i < Count; ++i)
	{
		ANPC_PlayerCharacter* NPC = GetWorld()->SpawnActor<ANPC_PlayerCharacter>(NPCClass, NPCSpawnPoint.Get()->GetTransform());

		if(NPC)
		{
			NPC->Init(CurrentSpawnParams);
			IdleNPCPlayers.Add(NPC);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn NPC"));
		}
	}
	
}

void AMainGameMode::StartGame_Implementation(int32 DifficultyLevel)
{
	// Set Parameters
	Level = DifficultyLevel;

	if(NPCSpawnParams.Num() > 0)
	{
		CurrentSpawnParams = NPCSpawnParams[DifficultyLevel - 1];
	}

	// Setup NPCs
	SpawnNPC(CurrentSpawnParams.TotalNPCs);

	OnGameStart.Broadcast();
}
