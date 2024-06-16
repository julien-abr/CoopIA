// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/PressurePlate.h"

#include "Kismet/KismetSystemLibrary.h"
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
		initialPos = object->GetActorLocation();
		if (!makeObjectDisappear)
		{
			object->SetActorLocation(FVector(initialPos.X, initialPos.Y, -5000));
		}
		object->SetActorEnableCollision(makeObjectDisappear);
	}
}

void APressurePlate::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if(OtherActor != nullptr)
	{
		if(!actorList.Contains(OtherActor))
			actorList.Add(OtherActor);

		if (actorList.Num() == 1)
		{
			OnPressure(OtherActor->GetActorLocation());

			if (object)
			{
				MoveObjectAnim(makeObjectDisappear);
				object->SetActorEnableCollision(!makeObjectDisappear);

				if (isUnique)
				{
					OnActorBeginOverlap.RemoveDynamic(this, &APressurePlate::OnOverlapBegin);
					OnActorEndOverlap.RemoveDynamic(this, &APressurePlate::OnOverlapEnd);
				}
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
			OnEndPressure();

			if (object)
			{
				object->SetActorEnableCollision(makeObjectDisappear);

				MoveObjectAnim(!makeObjectDisappear);
			}
		}
	}
}

void APressurePlate::MoveObjectAnim(bool IsDisappearing)
{
	USceneComponent* objectComponent = object->GetRootComponent();

	if(!IsDisappearing)
	{
		
		FLatentActionInfo latentInfoMesh;
		latentInfoMesh.CallbackTarget = objectComponent;
		UKismetSystemLibrary::MoveComponentTo(objectComponent, initialPos, object->GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoMesh);

	}
	else
	{
		FLatentActionInfo latentInfoMesh;
		latentInfoMesh.CallbackTarget = objectComponent;
		UKismetSystemLibrary::MoveComponentTo(objectComponent, FVector(initialPos.X, initialPos.Y, -5000), object->GetActorRotation(), false, true, _fallAnimTime, true, EMoveComponentAction::Move, latentInfoMesh);

	}
}