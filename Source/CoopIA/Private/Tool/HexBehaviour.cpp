// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/HexBehaviour.h"

#include <Components/ArrowComponent.h>

#include "Kismet/KismetSystemLibrary.h"

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
}
void AHexBehaviour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AHexBehaviour::GetMesh()
{
	return _hexMesh;
}

void AHexBehaviour::PreventAnim()
{
	FLatentActionInfo latentInfo;
	latentInfo.CallbackTarget = this;
	//FVector pos = _hexMesh->GetRelativeLocation() + FVector(FMath::RandRange(_minShake, _maxShake), FMath::RandRange(_minShake, _maxShake), FMath::RandRange(_minShake, _maxShake));
	//UKismetSystemLibrary::MoveComponentTo(_hexMesh, pos, GetActorRotation(), false, false, 0.2f, true, EMoveComponentAction::Move, latentInfo);
	UKismetSystemLibrary::MoveComponentTo(_hexMesh, _hexMesh->GetRelativeLocation() + FVector(0.f, 0.f, -500.f), GetActorRotation(), false, false, 0.2f, true, EMoveComponentAction::Move, latentInfo);
}

void AHexBehaviour::FallAnim()
{
	//GetWorld()->GetTimerManager().ClearTimer(_preventTimer);

	FVector pos = _hexMesh->GetRelativeLocation() + FVector(0, 0, -5000.f);

	FLatentActionInfo latentInfoMesh;
	latentInfoMesh.CallbackTarget = _hexMesh;
	UKismetSystemLibrary::MoveComponentTo(_hexMesh, pos, GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoMesh);

	FLatentActionInfo latentInfoCollider;
	latentInfoCollider.CallbackTarget = _hexCollider;
	UKismetSystemLibrary::MoveComponentTo(_hexCollider, pos, GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoCollider);
}