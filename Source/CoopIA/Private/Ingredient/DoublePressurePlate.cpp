// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/DoublePressurePlate.h"

ADoublePressurePlate::ADoublePressurePlate()
{
	secondBody = CreateDefaultSubobject<UStaticMeshComponent>("SecondBody");
	secondBody->SetupAttachment(RootComponent);

	triggerBox = CreateDefaultSubobject<UBoxComponent>("SecondTrigger");
	triggerBox->SetupAttachment(secondBody);
}