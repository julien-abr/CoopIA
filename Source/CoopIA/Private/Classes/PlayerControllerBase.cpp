// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/PlayerControllerBase.h"
#include "Classes/MainCamera.h"

void APlayerControllerBase::Init(AMainCamera* Camera)
{
	MainCamera = Camera;
	SetViewTargetWithBlend(MainCamera);
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetViewTargetWithBlend(MainCamera);
}
