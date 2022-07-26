// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

UENUM(BlueprintType)
enum class EDiceType : uint8
{
	Dealer = 0	UMETA(DisplayName = "Dealer"),
	Player = 1	UMETA(DisplayName = "Player"),
	Rigged = 2	UMETA(DisplayName = "Rigged")
};

UCLASS()
class THELOADEDDIE_API ADice : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* DiceMaterial;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DealerDiceColor;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor PlayerDiceColor;
	UPROPERTY(EditDefaultsOnly)
	TArray<FLinearColor> RiggedDiceColors;

public:	
	// Sets default values for this actor's properties
	ADice();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Call to set the color of the dice
	 */
	UFUNCTION(BlueprintCallable)
	void SetColor(EDiceType DiceType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
