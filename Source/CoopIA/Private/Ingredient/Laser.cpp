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

	ReflectLaser(start, end, count, this);
}

void ALaser::ReflectLaser(const FVector& start, const FVector& end, int count, AActor* actor)
{
	if (count >= 5)
		return;

	FCollisionQueryParams param;
	param.AddIgnoredActor(actor);

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, param))
	{
		if(hitResult.GetActor()->Implements<URayHit>())
		{
			IRayHit::Execute_RayHitAction(hitResult.GetActor());
		}
		else if(hitResult.GetActor()->ActorHasTag("Reflect"))
		{
			DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, FColor::Green, false, 0.1f, 0, 10.f);

			FVector hitPos = hitResult.ImpactPoint;
			FVector dir = (end - start).GetUnsafeNormal();
			FVector reflectDir = dir - 2 * UKismetMathLibrary::Dot_VectorVector(dir, hitResult.ImpactNormal) * hitResult.ImpactNormal;
			ReflectLaser(hitPos, hitPos + reflectDir * 10000, ++count, hitResult.GetActor());
		}
		else
		{
			DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 10.f);
		}

		return;
	}

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.1f, 0, 10.f);
}

//hitResult.GetActor()->IsA<>()
