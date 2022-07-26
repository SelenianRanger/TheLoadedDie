// Copyright © 2022 Sina Kamali 


#include "Actors/Dice.h"

// Sets default values
ADice::ADice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

}

// Called every frame
void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADice::SetColor(EDiceType DiceType)
{
	if(!DiceMaterial)
	{
		return;
	}
	
	FLinearColor FinalColor;
	switch (DiceType)
	{
	case EDiceType::Dealer:
        FinalColor = DealerDiceColor;
		break;

	case EDiceType::Player:
        FinalColor = PlayerDiceColor;
		break;

	case EDiceType::Rigged:
		if(RiggedDiceColors.Num() > 0)
		{
			int32 index = FMath::RandRange(0, RiggedDiceColors.Num() - 1);
			FinalColor = RiggedDiceColors[index];
		}
		break;

	default:
		break;
	}

	UMaterialInstanceDynamic* MaterialInstanceDyn = UMaterialInstanceDynamic::Create(DiceMaterial, Mesh);
	MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color"), FinalColor);
	Mesh->SetMaterial(0, MaterialInstanceDyn);
}

// Called when the game starts or when spawned
void ADice::BeginPlay()
{
	Super::BeginPlay();
	
}

