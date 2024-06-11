// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Laser.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/Interface/RayHit.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"

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

	if (!laserEffect)
		return;

	for(int i = 0; i < reflexionNbr; i++)
	{
		TObjectPtr<UNiagaraComponent> NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(laserEffect, _firePoint, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		NiagaraComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		_laserEffectArray.Add(NiagaraComp);
	}

	HideAllLaser(0);
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector start = _firePoint->GetComponentLocation();
	FVector end = start + _firePoint->GetForwardVector() * 10000;

	int count = 0;

	ReflectLaser(start, end, _body->GetComponentRotation().Yaw, count, this);
	UE_LOGFMT(LogTemp, Log, "{0}", count);
	HideAllLaser(count + 1);
}

void ALaser::ReflectLaser(const FVector& start, const FVector& end, float rotZ, int& count, AActor* actor)
{
	if (count >= reflexionNbr)
		return;

	FCollisionQueryParams param;
	param.AddIgnoredActor(actor);

	_laserEffectArray[count]->SetWorldLocation(start);

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, param))
	{
		_laserEffectArray[count]->SetVisibility(true);
		_laserEffectArray[count]->SetNiagaraVariableVec3(FString("Beam End"), hitResult.ImpactPoint);

		if(hitResult.GetActor()->Implements<URayHit>())
		{
			IRayHit::Execute_RayHitAction(hitResult.GetActor());
		}

		if(hitResult.GetActor()->ActorHasTag("Reflect"))
		{
			FVector hitPos = hitResult.ImpactPoint;
			FVector dir = (end - start).GetUnsafeNormal();
			FVector reflectDir = dir - 2 * UKismetMathLibrary::Dot_VectorVector(dir, hitResult.ImpactNormal) * hitResult.ImpactNormal;
			float z = UKismetMathLibrary::FindLookAtRotation(hitPos, hitPos + reflectDir * 10000).Yaw;
			
			ReflectLaser(hitPos, hitPos + reflectDir * 10000, z, ++count, hitResult.GetActor());
		}

		return;
	}
	_laserEffectArray[count]->SetVisibility(true);
	_laserEffectArray[count]->SetNiagaraVariableVec3(FString("Beam End"), end);
}

void ALaser::HideAllLaser(int count)
{
	for(int i = count; i < _laserEffectArray.Num(); i++)
	{
		_laserEffectArray[i]->SetVisibility(false);
	}
}
