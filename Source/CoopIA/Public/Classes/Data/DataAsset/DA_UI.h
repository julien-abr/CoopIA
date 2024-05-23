// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_UI.generated.h"

/**
 * 
 */
UCLASS()
class COOPIA_API UDA_UI : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		UMaterialInterface* RedIA_Mat0;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* RedIA_Mat1;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* GreenIA_Mat0;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* GreenIA_Mat1;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerGreenIA_Mat0;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerGreenIA_Mat1;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerGreenIA_Mat2;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerGreenIA_Mat3;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerRedIA_Mat0;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerRedIA_Mat1;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerRedIA_Mat2;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlayerRedIA_Mat3;
};
