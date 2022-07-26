// Copyright © 2022 Sina Kamali 


#include "Actors/Monitor.h"

#include "Engine/CurveTable.h"
#include "GameFramework/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AMonitor::BeginPlay()
{
	Super::BeginPlay();
	
	ReadStatsFromTable();

	GetWorld()->GetTimerManager().SetTimer(EventTimer, this, &AMonitor::TriggerEvent, FMath::RandRange(IdleCDMin, IdleCDMax));
	
}

void AMonitor::ReadStatsFromTable()
{
	if(!EventStatsTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Monitor stats table not set."));
		return;
	}
	
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if(!GameMode)
	{
		return;
	}

	// get idle event stats
	FRealCurve* Curve = EventStatsTable->FindCurve(TEXT("IdleCooldownMin"), "");
	if(Curve)
	{
		IdleCDMin = Curve->Eval(GameMode->Level);
	}
	Curve = EventStatsTable->FindCurve(TEXT("IdleCooldownMax"), "");
	if(Curve)
	{
		IdleCDMax = Curve->Eval(GameMode->Level);
	}

	// get power event stats
	Curve = EventStatsTable->FindCurve(TEXT("PowerCooldownMin"), "");
	if(Curve)
	{
		PowerCDMin = Curve->Eval(GameMode->Level);
	}
	Curve = EventStatsTable->FindCurve(TEXT("PowerCooldownMax"), "");
	if(Curve)
	{
		PowerCDMax = Curve->Eval(GameMode->Level);
	}
	Curve = EventStatsTable->FindCurve(TEXT("PowerChance"), "");
	if(Curve)
	{
		PowerChance = Curve->Eval(GameMode->Level);
	}

	// get random event stats
	Curve = EventStatsTable->FindCurve(TEXT("RandomCooldownMin"), "");
	if(Curve)
	{
		RandomCDMin = Curve->Eval(GameMode->Level);
	}
	Curve = EventStatsTable->FindCurve(TEXT("RandomCooldownMax"), "");
	if(Curve)
	{
		RandomCDMax = Curve->Eval(GameMode->Level);
	}
	Curve = EventStatsTable->FindCurve(TEXT("RandomChance"), "");
	if(Curve)
	{
		RandomChance = Curve->Eval(GameMode->Level);
	}
}

void AMonitor::TriggerEvent()
{
	float CDMin = IdleCDMin;
	float CDMax = IdleCDMax;

	if(bTurnedOn)
	{
		AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
		if(!GameMode)
		{
			return;
		}
	
		int32 ChannelsCount = GameMode->CameraChannels.Num();
		if(ChannelsCount < 3)
		{
			return;
		}

		if(FMath::RandRange(0.f, 1.f) < RandomChance)
		{
			CDMin = RandomCDMin;
			CDMax = RandomCDMax;

			GoRandomChannel();
		}
		
		if(FMath::RandRange(0.f, 1.f) < PowerChance)
		{
			CDMin = PowerCDMin;
			CDMax = PowerCDMax;

			bTurnedOn = false;
			OnChannelChanged.Broadcast(GameMode->CameraChannels[ChannelsCount - 1]);
		}
	}

	float FinalCD = FMath::RandRange(CDMin, CDMax);
	GetWorld()->GetTimerManager().SetTimer(EventTimer, this, &AMonitor::TriggerEvent, FinalCD);
}

void AMonitor::TogglePower()
{
	bTurnedOn = !bTurnedOn;

	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if(!GameMode)
	{
		return;
	}
	
	int32 ChannelsCount = GameMode->CameraChannels.Num();
	if(ChannelsCount < 3)
	{
		return;
	}
	
	if(bTurnedOn)
	{
		OnChannelChanged.Broadcast(GameMode->CameraChannels[ChannelIndex]);
	}
	else
	{
		OnChannelChanged.Broadcast(GameMode->CameraChannels[ChannelsCount - 1]);
	}
}

void AMonitor::GoNextChannel()
{
	if(!bTurnedOn)
	{
		return;;
	}
	
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if(!GameMode)
	{
		return;
	}
	
	int32 ChannelsCount = GameMode->CameraChannels.Num();
	if(ChannelsCount < 3)
	{
		return;
	}
	
	ChannelIndex = (ChannelIndex + 1) % (ChannelsCount - 2);
	OnChannelChanged.Broadcast(GameMode->CameraChannels[ChannelIndex]);
}

void AMonitor::GoRandomChannel()
{
	if(!bTurnedOn)
	{
		return;;
	}
	
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if(!GameMode)
	{
		return;
	}

	int32 ChannelsCount = GameMode->CameraChannels.Num();
	if(ChannelsCount < 3)
	{
		return;
	}
	
	ChannelIndex = FMath::RandRange(0, ChannelsCount - 3);
	OnChannelChanged.Broadcast(GameMode->CameraChannels[ChannelIndex]);
}

