// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/BuilderTool.h"

#include "Kismet/KismetMathLibrary.h"
#include <Components/ArrowComponent.h>

// Sets default values
ABuilderTool::ABuilderTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_ground = CreateDefaultSubobject<UStaticMeshComponent>("Ground");
	_ground->SetupAttachment(RootComponent);
	_ground->SetWorldLocation(FVector(0.f, 0.f, _height - 50.f));

	_arrow = CreateDefaultSubobject<UArrowComponent>("Direction");
	_arrow->SetupAttachment(_ground);
}

FVector ABuilderTool::HexCoord(const FVector& start, const FVector& end)
{
	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic))
	{
		float xHitPos = hitResult.ImpactPoint.X;
		int xRound = UKismetMathLibrary::Round(xHitPos / _xSpacing);

		int xModulo = xRound % 2;
		bool isEven = xModulo == 0;
		float addOffset = UKismetMathLibrary::SelectFloat(0, -260.f, isEven);

		float yHitPos = hitResult.ImpactPoint.Y;
		yHitPos = (yHitPos + addOffset) / _ySpacing;
		int yRound = UKismetMathLibrary::Round(yHitPos);

		xRound *= _xSpacing;
		yRound *= _ySpacing;
		float yRoundWithOffset = yRound + _offset;

		return UKismetMathLibrary::SelectVector(FVector(xRound, yRound, _height), FVector(xRound, yRoundWithOffset, _height), isEven);
	}

	return FVector();
}

AActor* ABuilderTool::BuildHex(const FVector& pos)
{
	AActor* spawnedHex = GetWorld()->SpawnActor<AActor>(_hexaPrefab, pos, FRotator(0.f, 0.f, 0.f));
	
	if(spawnedHex)
	{
#if WITH_EDITOR
		spawnedHex->SetFolderPath("Level_Hex");
#endif
		return spawnedHex;
	}

	return nullptr;
}

void ABuilderTool::ChangeHeight(const float& height)
{
	_height = height;
	_ground->SetWorldLocation(FVector(0.f, 0.f, _height - 50.f));
}

