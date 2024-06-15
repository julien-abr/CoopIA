// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/DoublePressurePlate.h"

ADoublePressurePlate::ADoublePressurePlate()
{
	triggerBox = CreateDefaultSubobject<UBoxComponent>("SecondTrigger");
	triggerBox->SetupAttachment(RootComponent);
}
