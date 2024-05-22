// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/PressurePlate.h"

#include "Logging/StructuredLog.h"

APressurePlate::APressurePlate()
{
	body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	body->SetupAttachment(RootComponent);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);

	if(object)
	{
		object->SetActorHiddenInGame(!makeObjectAppear);
		object->SetActorEnableCollision(makeObjectAppear);
	}
}

void APressurePlate::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if(OtherActor != nullptr)
	{
		if(!actorList.Contains(OtherActor))
			actorList.Add(OtherActor);

		if (object)
		{
			object->SetActorHiddenInGame(makeObjectAppear);
			object->SetActorEnableCollision(!makeObjectAppear);

			if (isUnique)
			{
				OnActorBeginOverlap.RemoveDynamic(this, &APressurePlate::OnOverlapBegin);
				OnActorEndOverlap.RemoveDynamic(this, &APressurePlate::OnOverlapEnd);
			}
		}
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
			if (object)
			{
				object->SetActorHiddenInGame(!makeObjectAppear);
				object->SetActorEnableCollision(makeObjectAppear);
			}
		}
	}
}
