// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/WallBreaking.h"

#include "Classes/Spear.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/StructuredLog.h"

// Sets default values
AWallBreaking::AWallBreaking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	wall = CreateDefaultSubobject<UStaticMeshComponent>("Wall");
	wall->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWallBreaking::BeginPlay()
{
	Super::BeginPlay();
	
	wall->OnComponentHit.AddDynamic(this, &AWallBreaking::OnHit);
}

// Called every frame
void AWallBreaking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWallBreaking::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->ActorHasTag("Spear"))
	{
		if(ASpear* spear = Cast<ASpear>(OtherActor))
		{
			if(spear->GetSpearState() == ESpearState::DASHING)
			{
				OnWallBreak();

				if(destroyActorPrefab)
				{
					AActor* actor = GetWorld()->SpawnActor<AActor>(destroyActorPrefab, GetActorLocation(), GetActorRotation());
					actor->SetLifeSpan(2.f);
				}

				Destroy();
			}
		}
	}
}