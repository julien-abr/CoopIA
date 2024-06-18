// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/WindCurrent.h"

#include <Classes/Shield.h>

#include "GameplayTagAssetInterface.h"
#include "Classes/CharacterBaseIA.h"
#include "Data/Interface/Interact.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

// Sets default values
AWindCurrent::AWindCurrent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	_cube->SetupAttachment(RootComponent);

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_cube);

	_windDirection = CreateDefaultSubobject<UArrowComponent>("WindDirection");
	_windDirection->SetupAttachment(_cube);
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


	FVector forceDirection = _cube->GetForwardVector() * windForce;

	for (int i = 0; i < shieldsInWind.Num(); i++)
	{
		windCurrentAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector(_windDirection->GetForwardVector().X, _windDirection->GetForwardVector().Y, 0), FVector(1, 0, 0))));
		if (_windDirection->GetForwardVector().Y < 0) windCurrentAngle = -windCurrentAngle;
		windShieldAngle = windCurrentAngle - shieldsInWind[i]->ShieldAngle;

		if (abs(windShieldAngle) >= 135 && abs(windShieldAngle) <= 225) forceDirection = FVector(0);

		//float windShieldAngle = GetDotProductTo(shieldsInWind[i]);
		//if (shieldsInWind[i]) forceDirection = FVector(0);

			//FVector shieldRotation = shieldsInWind[i]->GetActorRotation().Vector();
	} 
	
	
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

		if (Cast<AShield>(OtherActor))
		{
			shieldsInWind.Emplace(Cast<AShield>(OtherActor));
			/*FVector v1 = GetActorRotation().Vector();
			float dpt = FVector::DotProduct(FVector::ZeroVector, v1);
			dpt = FMath::Acos(dpt);*/
			/*windCurrentAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector(_windDirection->GetForwardVector().X, _windDirection->GetForwardVector().Y, 0), FVector(1,0,0))));
			if (_windDirection->GetForwardVector().Y < 0) windCurrentAngle = -windCurrentAngle;
			AShield* jield = Cast<AShield>(OtherActor);
			windShieldAngle = windCurrentAngle - jield->ShieldAngle;*/
		}
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
		if (Cast<AShield>(OtherActor)) shieldsInWind.RemoveSingle(Cast<AShield>(OtherActor));
	}
}

