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

	for(int i = 0; i < reflexionNbr; i++)
	{
		//UStaticMeshComponent* a = CreateDefaultSubobject<UStaticMeshComponent>(std::to_string(i).c_str());
		UStaticMeshComponent* meshComp = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), std::to_string(i).c_str());
		meshComp->RegisterComponent();
		meshComp->AttachToComponent(_firePoint, FAttachmentTransformRules::KeepRelativeTransform);
		meshComp->CreationMethod = EComponentCreationMethod::Instance;
		meshComp->SetStaticMesh(_laserCylinder);
		meshComp->SetRelativeLocation(FVector());
		meshComp->SetRelativeRotation(FRotator(-90, 0, 0));
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		_laserCylinderArray.Add(meshComp);
	}

	HideAllLaser();
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector start = _firePoint->GetComponentLocation();
	FVector end = start + _firePoint->GetForwardVector() * 10000;

	int count = 0;

	HideAllLaser();
	ReflectLaser(start, end, _body->GetComponentRotation().Yaw, count, this);
}

void ALaser::ReflectLaser(const FVector& start, const FVector& end, float rotZ, int count, AActor* actor)
{
	if (count >= reflexionNbr)
		return;

	FCollisionQueryParams param;
	param.AddIgnoredActor(actor);

	_laserCylinderArray[count]->SetWorldLocation(start);
	_laserCylinderArray[count]->SetWorldRotation(FRotator(-90, rotZ, 0));

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, param))
	{
		_laserCylinderArray[count]->SetVisibility(true);
		_laserCylinderArray[count]->SetWorldScale3D(FVector(laserSize, laserSize, (hitResult.Distance + 1) / 100));

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
	_laserCylinderArray[count]->SetVisibility(true);
	_laserCylinderArray[count]->SetWorldScale3D(FVector(laserSize, laserSize, (end.Length() + 1) / 100));
}

void ALaser::HideAllLaser()
{
	for(int i = 0; i < _laserCylinderArray.Num(); i++)
	{
		_laserCylinderArray[i]->SetVisibility(false);
	}
}

//hitResult.GetActor()->IsA<>()
