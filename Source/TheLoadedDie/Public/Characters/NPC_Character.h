// Copyright © 2022 Sina Kamali 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_Character.generated.h"

USTRUCT()
struct FColorPalette 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color01;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color02;
};

UCLASS()
class THELOADEDDIE_API ANPC_Character : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* AntennaMesh;

	// customization properties
	//-----------------------------
	UPROPERTY(EditDefaultsOnly, Category="Customization | Base Materials")
	class UMaterialInterface* HeadDetailMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Base Materials")
	class UMaterialInterface* HeadBaseMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Base Materials")
	class UMaterialInterface* BodyBaseMaterial;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Base Materials")
	class UMaterialInterface* BodyHighlightMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="Customization | Colors")
	TArray<class UTexture2D*> HeadDetailMasks;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Colors")
	TArray<FColorPalette> HeadDetailColors;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Colors")
	TArray<FLinearColor> HeadBaseColors;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Colors")
	TArray<FLinearColor> BodyBaseColors;
	UPROPERTY(EditDefaultsOnly, Category="Customization | Colors")
	TArray<FLinearColor> BodyHighlightColors;

	UPROPERTY(VisibleDefaultsOnly, Category="Customization")
	bool bHasAntenna;
	// -----------------------------
	
public:
	// Sets default values for this character's properties
	ANPC_Character();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Randomizes colors, textures and details of the character mesh
	 */
	UFUNCTION(BlueprintCallable)
	void RandomizeCustomization();
	
};
