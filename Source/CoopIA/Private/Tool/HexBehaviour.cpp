// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/HexBehaviour.h"

#include <Components/ArrowComponent.h>

#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"

AHexBehaviour::AHexBehaviour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_arrowRoot = CreateDefaultSubobject<UArrowComponent>("Root");
	RootComponent = _arrowRoot;
	_arrowRoot->SetVisibility(false);

	_arrowMesh = CreateDefaultSubobject<UArrowComponent>("HexParent");
	_arrowMesh->SetupAttachment(_arrowRoot);
	_arrowMesh->SetWorldLocation(FVector(-300.f, 0.f, 0.f));
	_arrowMesh->SetVisibility(false);

	_hexCollider = CreateDefaultSubobject<UStaticMeshComponent>("HexCollider");
	_hexCollider->SetupAttachment(_arrowMesh);
	_hexCollider->SetVisibility(false);

	_hexMesh = CreateDefaultSubobject<UStaticMeshComponent>("HexMesh");
	_hexMesh->SetupAttachment(_arrowMesh);
}
void AHexBehaviour::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorTickEnabled(false);

	UMaterialInterface* matFloor = _hexMesh->GetMaterial(0);
	_dMatFloor = UKismetMaterialLibrary::CreateDynamicMaterialInstance(nullptr, matFloor);
	_hexMesh->SetMaterial(0, _dMatFloor);
	UMaterialInterface* matGrass = _hexMesh->GetMaterial(1);
	_dMatGrass = UKismetMaterialLibrary::CreateDynamicMaterialInstance(nullptr, matGrass);
	_hexMesh->SetMaterial(1, _dMatGrass);

	_dMatFloorDisolve = UKismetMaterialLibrary::CreateDynamicMaterialInstance(nullptr, disolveMat);
	_dMatGrassDisolve = UKismetMaterialLibrary::CreateDynamicMaterialInstance(nullptr, disolveMat);
}

void AHexBehaviour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_dMatTake != nullptr)
	{
		if(!_mustDissolve)
		{
			_timer += DeltaTime;
			_dMatTake->SetVectorParameterValue("Color_1", UKismetMathLibrary::LinearColorLerp(_colorInit, _colorTake, _timer));

			if(_timer > 1)
			{
				_timer = 0;
				_dMatTake = nullptr;

				if (_state == 1)
				{
					_hexMesh->SetMaterial(0, _dMatFloorDisolve);
					FTimerDelegate Delegate;
					Delegate.BindUObject(this, &AHexBehaviour::DissolveMaterial, _dMatFloorDisolve);
					GetWorld()->GetTimerManager().SetTimer(_preventTimer, Delegate, FMath::RandRange(0.f, 0.2f), false);
				}
			}
		}
		else
		{
			_timer += DeltaTime;
			_dMatTake->SetScalarParameterValue("Dissolve", UKismetMathLibrary::Lerp(0.5f, -0.3f, _timer));

			if (_timer > 1)
			{
				_timer = 0;
				_dMatTake = nullptr;

				_state++;
				if(_state == 1)
				{
					FTimerDelegate Delegate;
					Delegate.BindUObject(this, &AHexBehaviour::ChangeMaterialColor, _dMatFloor, secondColorCorruption);
					GetWorld()->GetTimerManager().SetTimer(_preventTimer, Delegate, FMath::RandRange(0.f, 0.2f), false);
				}
				else if(_state == 2)
				{
					this->SetActorTickEnabled(false);
				}
			}
		}
	}
}

const FVector AHexBehaviour::GetRespawnLoc() const
{
	return _arrowMesh->GetComponentLocation() + (FVector::UpVector * 100);
}

UStaticMeshComponent* AHexBehaviour::GetMesh()
{
	return _hexMesh;
}

void AHexBehaviour::FirstPreventAnim()
{
	this->SetActorTickEnabled(true);

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &AHexBehaviour::ChangeMaterialColor, _dMatGrass, firstColorCorruption);
	GetWorld()->GetTimerManager().SetTimer(_preventTimer, Delegate, FMath::RandRange(0.f, 0.2f), false);
}

void AHexBehaviour::SecondPreventAnim()
{
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &AHexBehaviour::ChangeMaterialColor, _dMatGrass, secondColorCorruption);
	GetWorld()->GetTimerManager().SetTimer(_preventTimer, Delegate, FMath::RandRange(0.f, 0.2f), false);
}

void AHexBehaviour::FallAnim()
{
	_isDead = true;

	if (!_dMatGrassDisolve || !_dMatFloorDisolve)
		return;

	_hexMesh->SetMaterial(1, _dMatGrassDisolve);
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &AHexBehaviour::DissolveMaterial, _dMatGrassDisolve);
	GetWorld()->GetTimerManager().SetTimer(_preventTimer, Delegate, FMath::RandRange(0.f, 0.2f), false);

	FVector pos = _hexMesh->GetRelativeLocation() + FVector(0, 0, -5000.f);

	FLatentActionInfo latentInfoMesh;
	latentInfoMesh.CallbackTarget = _hexMesh;
	UKismetSystemLibrary::MoveComponentTo(_hexMesh, pos, GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoMesh);

	FLatentActionInfo latentInfoCollider;
	latentInfoCollider.CallbackTarget = _hexCollider;
	UKismetSystemLibrary::MoveComponentTo(_hexCollider, pos, GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoCollider);
}

void AHexBehaviour::ChangeMaterialColor(TObjectPtr<UMaterialInstanceDynamic> dynMat, FLinearColor color)
{
	_dMatTake = dynMat;
	FHashedMaterialParameterInfo a;
	a.Name = FScriptName("Color_1");
	dynMat->GetVectorParameterValue(a, _colorInit);
	_colorTake = color;
	_mustDissolve = false;
}

void AHexBehaviour::DissolveMaterial(TObjectPtr<UMaterialInstanceDynamic> dynMat)
{
	_dMatTake = dynMat;
	_mustDissolve = true;
}
