// Copyright © 2022 Sina Kamali 

#include "Actors/GameTable.h"

#include "Characters/NPC_Character.h"
#include "Characters/NPC_PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameTable::AGameTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table Mesh"));
	SetRootComponent(TableMesh);

	DealerSpawnBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Dealer Spawn"));
	DealerSpawnBillboard->SetupAttachment(RootComponent);
	PlayerSpawnBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Player Spawn"));
	PlayerSpawnBillboard->SetupAttachment(RootComponent);

	PlayerDiceSpawnBillboard = CreateDefaultSubobject<UBillboardComponent>("Player Dice Spawn");
	PlayerDiceSpawnBillboard->SetupAttachment(RootComponent);
	PlayerDiceArrow = CreateDefaultSubobject<UArrowComponent>("PLayer Dice Throw Direction");
	PlayerDiceArrow->SetupAttachment(PlayerDiceSpawnBillboard);
	DealerDiceSpawnBillboard = CreateDefaultSubobject<UBillboardComponent>("Dealer Dice Spawn");
	DealerDiceSpawnBillboard->SetupAttachment(RootComponent);
	DealerDiceArrow = CreateDefaultSubobject<UArrowComponent>("Dealer Dice Throw Direction");
	DealerDiceArrow->SetupAttachment(DealerDiceSpawnBillboard);
	
	DealerSlotCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Dealer Slot"));
	DealerSlotCollision->SetupAttachment(RootComponent);
	PlayerSlotCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Slot"));
	PlayerSlotCollision->SetupAttachment(RootComponent);
	
}

// Called every frame
void AGameTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameTable::PlayMatch_Implementation(FMatchResult& MatchResult)
{
	if(!bIsGameInProgress)
	{
		MatchResult = FMatchResult();
		return;
	}

	MatchResult = FMatchResult();
	ANPC_PlayerCharacter* Player = Cast<ANPC_PlayerCharacter>(PlayerNPC);
	if(!Player)
	{
		return;
	}
	
	MatchResult.Bet = Player->GetNextBet();

	MatchResult.PlayerDiceOneRoll = Player->Dice->Roll();
	MatchResult.PlayerDiceTwoRoll = Player->Dice->Roll();
	MatchResult.DealerDiceOneRoll = DiceD6->Roll();
	MatchResult.DealerDiceTwoRoll = DiceD6->Roll();
	
	// PLayer must roll higher than 7 and also beat the dealer to win
	int32 PlayerSum = MatchResult.PlayerDiceOneRoll + MatchResult.PlayerDiceTwoRoll;
	int32 DealerSum = MatchResult.DealerDiceOneRoll + MatchResult.DealerDiceTwoRoll;
	MatchResult.PlayerWon =  PlayerSum > 7 && PlayerSum > DealerSum;

	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
	{
		GameMode->CheatersWinCount += Player->bIsCheater && MatchResult.PlayerWon ? 1 : 0;
		GameMode->CheatersLoseCount += Player->bIsCheater && !MatchResult.PlayerWon ? 1 : 0;
	}

	Player->Balance += MatchResult.Bet * (MatchResult.PlayerWon ? 1 : -1);

	OnNewMatchEndedInternal.Broadcast(MatchResult);
}

// Called when the game starts or when spawned
void AGameTable::BeginPlay()
{
	Super::BeginPlay();

	OnNewMatchEndedInternal.AddLambda([&](FMatchResult MatchResult)
	{
		OnNewMatchEnded.Broadcast(MatchResult);
	});

	DiceD6 = NewObject<UDiceD6>(this, UDiceD6::StaticClass());
	DiceD6->SetDiceType(ELoadedDiceType::ELDT_NotLoaded);
}

