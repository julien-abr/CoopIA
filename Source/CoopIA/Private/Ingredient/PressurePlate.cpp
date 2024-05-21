// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/PressurePlate.h"

#include "Logging/StructuredLog.h"

APressurePlate::APressurePlate()
{
	_body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	_body->SetupAttachment(RootComponent);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);

	object->SetActorHiddenInGame(!makeObjectAppear);
	object->SetActorEnableCollision(makeObjectAppear);
}

void APressurePlate::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if(OtherActor != nullptr)
	{
		if(!actorList.Contains(OtherActor))
			actorList.Add(OtherActor);

		object->SetActorHiddenInGame(makeObjectAppear);
		object->SetActorEnableCollision(!makeObjectAppear);
	}
}

void APressurePlate::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor != nullptr)
	{
		if (actorList.Contains(OtherActor))
			actorList.Remove(OtherActor);

		if(actorList.Num() == 0)
		{
			object->SetActorHiddenInGame(!makeObjectAppear);
			object->SetActorEnableCollision(makeObjectAppear);
		}
	}
}
