// Copyright © 2022 Sina Kamali 

#include "GameFramework/MainGameMode.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


ANPC_PlayerCharacter* AMainGameMode::AskForNPCPlayer()
{
	for (ANPC_PlayerCharacter* NPC : IdleNPCPlayers)
	{
		if(FMath::RandRange(0.f, 1.f) < NPCAnswerRates[Level - 1])
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
		ReplaceNPC(NPC);
		return;
	}
	
	if(NPC->WantsToLeave())
	{
		CheatersMissed += NPC->bIsCheater ? 1 : 0;
		CheatersProfit += NPC->bIsCheater ? NPC->Balance - NPC->InitialBalance : 0;
		CheatersProfitPercent += NPC->bIsCheater ? NPC->AccumulatedProfit : 0;
		ReplaceNPC(NPC);
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

void AMainGameMode::ReplaceNPC(ACharacter* NPC)
{
	NPC->Destroy();

	ANPC_PlayerCharacter* NewNPC = GetWorld()->SpawnActor<ANPC_PlayerCharacter>(NPCClass, NPCSpawnPoint.Get()->GetTransform());

	if(NewNPC)
	{
		NewNPC->bIsCheater = FMath::RandRange(0.f, 1.f) < DistributionParameters.CheaterChance;
		NewNPC->bIsSaboteur = NewNPC->bIsCheater && FMath::RandRange(0.f, 1.f) < 0.3;
		NewNPC->bIsStupid = FMath::RandRange(0.f, 1.f) < DistributionParameters.StupidChance;
		if(FMath::RandRange(0.f, 1.f) < DistributionParameters.RichChance)
		{
			NewNPC->Balance = FMath::RandRange(RichBalanceRange.MinBalance, RichBalanceRange.MaxBalance);
		}
		else
		{
			NewNPC->Balance = FMath::RandRange(NormalBalanceRange.MinBalance, NormalBalanceRange.MaxBalance);
		}
		NewNPC->InitialBalance = NewNPC->Balance;

		NewNPC->Init();
		
		IdleNPCPlayers.Add(NewNPC);
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
	bool* CaptureOn = new bool[CameraChannels.Num()];
	for (int i = 0; i < Monitors.Num(); ++i)
	{
		CaptureOn[Monitors[i]->ChannelIndex] = Monitors[i]->bTurnedOn;
	}

	for(int i = 0; i < CameraChannels.Num(); ++i)
	{
		if(CameraChannels[i].CCTV.IsValid())
		{
			CameraChannels[i].CCTV.Get()->SetActorTickEnabled(CaptureOn[i]);
		}
	}
}

void AMainGameMode::SpawnNPCs()
{
	if(!NPCSpawnPoint.IsValid())
	{
		return;
	}
	
	for (int i = 0; i < DistributionParameters.TotalNPCs; ++i)
	{
		ANPC_PlayerCharacter* NPC = GetWorld()->SpawnActor<ANPC_PlayerCharacter>(NPCClass, NPCSpawnPoint.Get()->GetTransform());

		if(NPC)
		{
			NPC->bIsCheater = FMath::RandRange(0.f, 1.f) < DistributionParameters.CheaterChance;
			NPC->bIsSaboteur = NPC->bIsCheater && FMath::RandRange(0.f, 1.f) < 0.3;
			NPC->bIsStupid = FMath::RandRange(0.f, 1.f) < DistributionParameters.StupidChance;
			if(FMath::RandRange(0.f, 1.f) < DistributionParameters.RichChance)
			{
				NPC->Balance = FMath::RandRange(RichBalanceRange.MinBalance, RichBalanceRange.MaxBalance);
			}
			else
			{
				NPC->Balance = FMath::RandRange(NormalBalanceRange.MinBalance, NormalBalanceRange.MaxBalance);
			}
			NPC->InitialBalance = NPC->Balance;

			NPC->Init();
		
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

	if(NPCDistributions.Num() > 0)
	{
		DistributionParameters = NPCDistributions[DifficultyLevel - 1];
	}

	// Setup NPCs
	SpawnNPCs();
}
