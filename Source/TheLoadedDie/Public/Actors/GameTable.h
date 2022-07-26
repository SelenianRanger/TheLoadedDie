// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "AI/DiceD6.h"
#include "GameFramework/Pawn.h"
#include "GameTable.generated.h"

USTRUCT(BlueprintType)
struct FMatchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerDiceOneRoll = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerDiceTwoRoll = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 DealerDiceOneRoll = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 DealerDiceTwoRoll = 0;

	UPROPERTY(BlueprintReadWrite)
	bool PlayerWon = false;

	UPROPERTY(BlueprintReadWrite)
	int32 Bet = 0;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMatchEndedDynamicDelegate, FMatchResult, MatchResult);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewMatchEndedDelegate, FMatchResult);

UCLASS()
class THELOADEDDIE_API AGameTable : public APawn
{
	GENERATED_BODY()

public:

	// Components
	// ---------------------------------------
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBillboardComponent* DealerSpawnBillboard;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBillboardComponent* PlayerSpawnBillboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBillboardComponent* PlayerDiceSpawnBillboard;
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* PlayerDiceArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBillboardComponent* DealerDiceSpawnBillboard;
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* DealerDiceArrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* DealerSlotCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* PlayerSlotCollision;
	// ---------------------------------------

	UPROPERTY(BlueprintAssignable)
	FOnNewMatchEndedDynamicDelegate OnNewMatchEnded;

	FOnNewMatchEndedDelegate OnNewMatchEndedInternal;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bIsRigged = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bIsDealerOnBreak = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool bIsGameInProgress = false;

	UPROPERTY(BlueprintReadWrite)
	class ACharacter* DealerNPC;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	class ACharacter* PlayerNPC;

	UPROPERTY(BlueprintReadOnly)
	UDiceD6* DiceD6;
	
public:	
	// Sets default values for this actor's properties
	AGameTable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Call to spawn and throw dice at the table
	 * @param bIsDiceRigged: Whether the player is using rigged colored die
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ThrowDice(bool bIsDiceRigged = false);

	/**
	 * Orders the dealer to take a break if it's appropriate
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AskDealerToTakeBreak();

	/**
	 * Asks for a player to take the table slot
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RequestNewPlayer();

	/**
	 * Moves the player to table slot
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MovePlayerToSlot();

	/**
	 * Moves the player to sabotage slot
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MovePlayerToSabotage();
	
	/**
	 * Orders the player to leave the table
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OrderPlayerToLeave(bool bIsArrest = false);

	/**
	 * arrest the player and order them to leave
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ArrestPlayer();

	/**
	 * play a match if appropriate
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayMatch(FMatchResult& MatchResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
