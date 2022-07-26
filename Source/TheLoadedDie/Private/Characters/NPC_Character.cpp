// Copyright © 2022 Sina Kamali 


#include "Characters/NPC_Character.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ANPC_Character::ANPC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AntennaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Antenna"));
	AntennaMesh->SetupAttachment(GetMesh());

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AntennaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
}

// Called every frame
void ANPC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when the game starts or when spawned
void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC_Character::RandomizeCustomization()
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if(!SkeletalMesh)
	{
		return;
	}

	if(HeadDetailMaterial && HeadDetailMasks.Num() > 0 && HeadDetailColors.Num() > 0)
	{
		int32 index = FMath::RandRange(0, HeadDetailMasks.Num() - 1);
		UMaterialInstanceDynamic* MaterialInstanceDyn = UMaterialInstanceDynamic::Create(HeadDetailMaterial, SkeletalMesh);
		MaterialInstanceDyn->SetTextureParameterValue(TEXT("Tex"), HeadDetailMasks[index]);
		
		index = FMath::RandRange(0, HeadDetailColors.Num() - 1);
		if(FMath::RandBool())
		{
			MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color01"), HeadDetailColors[index].Color01);
			MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color02"), HeadDetailColors[index].Color02);
		}
		else
		{
			MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color01"), HeadDetailColors[index].Color02);
			MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color02"), HeadDetailColors[index].Color01);
		}
		
		SkeletalMesh->SetMaterialByName(TEXT("HeadDetail"), MaterialInstanceDyn);
	}

	if(HeadBaseMaterial && HeadBaseColors.Num() > 0)
	{
		int32 index = FMath::RandRange(0, HeadBaseColors.Num() - 1);
		UMaterialInstanceDynamic* MaterialInstanceDyn = UMaterialInstanceDynamic::Create(HeadBaseMaterial, SkeletalMesh);
		MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color"), HeadBaseColors[index]);
		SkeletalMesh->SetMaterialByName(TEXT("HeadBody"), MaterialInstanceDyn);
	}

	if(BodyBaseMaterial && BodyBaseColors.Num() > 0)
	{
		int32 index = FMath::RandRange(0, BodyBaseColors.Num() - 1);
		UMaterialInstanceDynamic* MaterialInstanceDyn = UMaterialInstanceDynamic::Create(BodyBaseMaterial, SkeletalMesh);
		MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color"), BodyBaseColors[index]);
		SkeletalMesh->SetMaterialByName(TEXT("BodyBase"), MaterialInstanceDyn);
	}

	if(BodyHighlightMaterial && BodyHighlightColors.Num() > 0)
	{
		int32 index = FMath::RandRange(0, BodyHighlightColors.Num() - 1);
		UMaterialInstanceDynamic* MaterialInstanceDyn = UMaterialInstanceDynamic::Create(BodyHighlightMaterial, SkeletalMesh);
		MaterialInstanceDyn->SetVectorParameterValue(TEXT("Color"), BodyHighlightColors[index]);
		SkeletalMesh->SetMaterialByName(TEXT("BodyLight"), MaterialInstanceDyn);
	}

	bHasAntenna = FMath::RandBool();
	AntennaMesh->SetVisibility(bHasAntenna);
}

