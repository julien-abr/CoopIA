// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/WindCurrent.h"

#include <Classes/Shield.h>

#include "GameplayTagAssetInterface.h"
#include "Classes/CharacterBaseIA.h"
#include "Data/Interface/Interact.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWindCurrent::AWindCurrent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	_cube->SetupAttachment(RootComponent);

	/*_cylinder = CreateDefaultSubobject<UStaticMeshComponent>("Cylinder");
	_cylinder->SetupAttachment(_cube);*/

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_cube);
}

// Called when the game starts or when spawned
void AWindCurrent::BeginPlay()
{
	Super::BeginPlay();

	_box->OnComponentBeginOverlap.AddDynamic(this, &AWindCurrent::OnBoxBeginOverlap);
	_box->OnComponentEndOverlap.AddDynamic(this, &AWindCurrent::OnBoxEndOverlap);
	
}

// Called every frame
void AWindCurrent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!inTheCurrent) return;

	for (int i = 0; i < shieldsInWind.Num(); i++)
	{
		if (shieldsInWind[i])
			FVector shieldRotation = shieldsInWind[i]->GetActorRotation().Vector();
	} 

	FVector forceDirection = _cube->GetForwardVector() * windForce;

	
	
	for (int i = 0; i < actorsInWind.Num(); i++)
	{
		
		if (actorsInWind[i])
			InteractInterfaces[i]->Execute_Wind(actorsInWind[i], forceDirection);

	}
}

void AWindCurrent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	{
		inTheCurrent = true;
		InteractInterfaces.Emplace(Cast<IInteract>(OtherActor));
		actorsInWind.Emplace(OtherActor);

		if (Cast<AShield>(OtherActor)) shieldsInWind.Emplace(OtherActor);
	}
}

void AWindCurrent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	{
		if (!Cast<ACharacterBaseIA>(OtherActor)) inTheCurrent = false;
		InteractInterfaces.RemoveSingle(Cast<IInteract>(OtherActor));
		actorsInWind.RemoveSingle(OtherActor);
		if (Cast<AActor>(OtherActor)) shieldsInWind.RemoveSingle(OtherActor);
	}
}

