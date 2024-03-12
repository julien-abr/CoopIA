// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Laser.h"

#include "Components/ArrowComponent.h"
#include "Data/Interface/RayHit.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	_body->SetupAttachment(RootComponent);
	_body->SetWorldScale3D(FVector(0.5f, 0.1f, 0.1f));

	_firePoint = CreateDefaultSubobject<UArrowComponent>("FirePoint");
	_firePoint->SetupAttachment(_body);
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector start = _firePoint->GetComponentLocation();
	FVector end = start + _firePoint->GetForwardVector() * 10000;

	int count = 0;

	ReflectLaser(start, end, count);
}

void ALaser::ReflectLaser(const FVector& start, const FVector& end, int count)
{
	if (count >= 5)
		return;

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic))
	{
		DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, FColor::Green, false, 0.1f, 0, 10.f);

		if(UKismetSystemLibrary::DoesImplementInterface(hitResult.GetActor(), URayHit::StaticClass()))
		{
			IRayHit::Execute_RayHitAction(hitResult.GetActor());
			return;
		}
		else
		{
			FVector hitPos = hitResult.ImpactPoint;
			FVector reflectDir = end - 2 * UKismetMathLibrary::Dot_VectorVector(end, hitResult.ImpactNormal) * hitResult.ImpactNormal;
			ReflectLaser(hitPos, reflectDir, ++count);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.1f, 0, 10.f);
	}
}

